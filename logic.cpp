#include <fstream>
#include "hash_md5/md5wrapper.h"
#include "logic.h"

int of::logic::remove_directory(const std::string &dir_name) {
    boost::system::error_code code;
    boost::filesystem::remove_all(dir_name, code);
    if(code){
        return -1;
    }
    return 0;
}

void of::logic::logging_error(const std::string error_msg) {
    if(!log_file){
        return;
    }
    std::string log_string = "[";

    time_t curr_time;
    time(&curr_time);
    std::string time = ctime(&curr_time);
    time.erase(time.end()-1);

    log_string.append(time);
    log_string.append("]");
    log_string.append(error_msg);
    log_string.append("\n");

    log_file << log_string;
    log_file.rdbuf()->pubsync();
}

int of::logic::create_directory(const std::string &dir_name) {
    boost::system::error_code code;
    auto file_stat = boost::filesystem::status(dir_name);

    if (file_stat.type() == boost::filesystem::file_not_found) {
        boost::filesystem::create_directories(dir_name, code);
        if (code) {
            logging_error("ERROR WHILE CREATING DIRECTORY! CHANGE OWNER OF CURRENT DIR");
            return -1;
        }
        file_stat = boost::filesystem::status(dir_name);
    }
    if (file_stat.type() != boost::filesystem::directory_file) {
        logging_error("ERROR " + dir_name + " NOT DIRECTORY! REMOVE FILE");
        return -1;
    }

    boost::filesystem::permissions(dir_name, boost::filesystem::no_perms, code);
    boost::filesystem::permissions(dir_name, boost::filesystem::owner_all | boost::filesystem::group_read | boost::filesystem::others_read | boost::filesystem::group_exe|boost::filesystem::others_exe, code);
    if (code) {
        logging_error("ERROR WHILE CHANGING PERMISSION! CHANGE OWNER OF " + dir_name + " DIR");
        return -1;
    }
    return 0;
}

void of::logic::create_symlink(file_info &file_inf, file_secret &secret) {
    if(create_directory(info_.nginx_directory+"/"+std::to_string(secret.id)) !=0 ||
    create_directory(info_.nginx_directory+"/"+ std::to_string(secret.id) +"/"+ secret.secret) !=0){
        logging_error("Error while creating symlink dirs" + file_inf.token);
        return;
    }
    boost::system::error_code code;

    boost::filesystem::create_symlink("../../../"+info_.storage_directory+"/"+file_inf.hash+"_"+file_inf.token, info_.nginx_directory + "/" + std::to_string(secret.id) +"/"+ secret.secret +"/"+ file_inf.filename, code);
    if(code){
        logging_error("Error while creating symlink" + file_inf.token);
        return;
    }
}


of::logic::logic(const of::server_logic_info &info) : info_(info) {
    log_file.open(info.log_file, std::ios_base::app);
    std::string buffer_log = "[";
    time_t curr_time;
    time(&curr_time);
    std::string time = ctime(&curr_time);
    time.erase(time.end()-1);
    buffer_log = buffer_log + time +"]----- STARTED SERVER ------\n";


    log_file << buffer_log;

    log_file.rdbuf()->pubsync();

    if(!log_file){
        std::cerr << "ERROR WHILE OPENING LOG FILE" << std::endl;
    }
    try{
        master_db_obj.open_db(info_.db_file);
    } catch (of::db_exceptions exc) {
        logging_error("DB error: " + exc.error_msg);
        exit(-1);
    }

    std::vector<std::string> dirs_to_create;
    dirs_to_create.push_back(info_.tmp_directory);
    dirs_to_create.push_back(info_.storage_directory);
    dirs_to_create.push_back(info_.nginx_directory);

    for (size_t i = 0; i < dirs_to_create.size(); ++i) {
        if (create_directory(dirs_to_create[i]) != 0) {
            exit(-1);
        }
    }

}

sqlite3 * of::logic::get_master_db_ptr() {
    return master_db_obj.get_db_ptr();
}

of::logic::~logic() {
    master_db_obj.close_db();
    log_file.close();
}

void of::logic::process_fetched_chunk(of::json_processing_data &data, std::string &part_file) {
    try {
        of::part_info part_inf;

        part_inf.token = data.input_json["token"];
        part_inf.partcount = data.input_json["part_num"];
        part_inf.active_type = UPLOAD;

        data.db_to_operate.get_or_set_file_part(part_inf, false);
        if (part_inf.partsize != part_file.length()) {
            data.error = true;
            data.error_msg = "Prislal bolshe tshem nado\n";
            logging_error("Part file size incorrect:" + part_inf.token
            + std::to_string(part_inf.partcount) + "; needed " + std::to_string(part_inf.partsize)
            + "; received "+ std::to_string(part_file.size()));
            return;
        }

        std::ofstream output_file;
        std::string token = data.input_json["token"];
        std::string part_num = to_string(data.input_json["part_num"]);
        output_file.open(info_.tmp_directory + "/" + token + "/" + part_num + ".part", std::ios_base::binary);
        output_file << part_file;
        output_file.close();

        data.db_to_operate.get_or_set_file_part(part_inf, true);

        data.ouput_json["status"] = 0;

        std::vector<size_t> remain_parts;
        data.db_to_operate.get_remain_parts_of_file(token, remain_parts);


        std::vector<size_t> tmp_parts_remain;
        tmp_parts_remain.clear();
        if(!remain_parts.empty()){
            tmp_parts_remain.push_back(remain_parts[0]);
        }

        data.ouput_json["partstoupload"] = tmp_parts_remain;

        data.glue_needing = false;
        if(remain_parts.empty()){
            data.glue_needing = true;

            of::file_info file_inf;
            file_inf.token = token;
            data.db_to_operate.get_active_file_info(file_inf, false);
            data.db_to_operate.add_stored_file(file_inf);
        }


        data.ouput_str = to_string(data.ouput_json);
    } catch (std::exception &e) {
        data.error_msg = e.what();
        std::string error = "Exception: " + data.error_msg;
        logging_error(error);
    }
}

void of::logic::process_json(of::json_processing_data &data) {
    try {
        data.ouput_json.clear();
        data.input_json = nlohmann::json::parse(data.input_str);
        data.bytes_waiting = 0;
        int action = data.input_json["action"];
        switch (action / 10) {
            case 1:
                process_upload(data);
                break;
            case 2:
                process_download(data);
                break;
        }

        if(data.ouput_str.empty()){
            data.ouput_str = to_string(data.ouput_json);
            return;
        }

        std::string bufferstr = data.ouput_str;
        data.ouput_str.clear();
        data.ouput_str = to_string(data.ouput_json);
        data.ouput_str.append(bufferstr);

    } catch (std::exception &e) {
        data.error = true;
        data.error_msg = e.what();
        std::string error = "Exception: " + data.error_msg;
        logging_error(error);
    }
}

void of::logic::add_new_file(of::json_processing_data &data) {
    of::file_info input_file;
    input_file.filename = data.input_json["file_name"];
    input_file.filesize = data.input_json["file_size"];
    input_file.hash = data.input_json["uniq"];
    input_file.partnums = data.input_json["part_num"];
    input_file.partsize = data.input_json["part_size"];
    input_file.active_type = UPLOAD;

//    size_t prev_part_size = input_file.partsize;

    data.db_to_operate.add_active_file(input_file);

//    if(prev_part_size != input_file.partsize){
//
//    }

    if (create_directory(info_.tmp_directory + "/" + input_file.token) != 0) {
        data.error = true;
        data.error_msg = "Error creating directory to part files";
        logging_error(data.error_msg + " "+ input_file.token);
    }

    data.ouput_json["token"] = input_file.token;

    std::vector<size_t> tmp_parts_remain;
    tmp_parts_remain.clear();
    if(!input_file.partremain.empty()){
        tmp_parts_remain.push_back(input_file.partremain[0]);
    }

    data.ouput_json["partstoupload"] = tmp_parts_remain;
    data.ouput_json["status"] = 0;

}

void of::logic::process_chunk_request(of::json_processing_data &data) {
    of::part_info part_file;
    part_file.token = data.input_json["token"];
    part_file.partcount = data.input_json["part_num"];
    part_file.active_type = UPLOAD;

    data.db_to_operate.get_or_set_file_part(part_file, false);

    if (part_file.partsize > 0 && part_file.active_type == UPLOAD) {
        data.bytes_waiting = part_file.partsize;
    } else {
        data.ouput_json["status"] = 2;
        std::vector<size_t> parts_remain;
        data.db_to_operate.get_remain_parts_of_file(part_file.token, parts_remain);

        std::vector<size_t> tmp_parts_remain;
        tmp_parts_remain.clear();
        if(!parts_remain.empty()){
            tmp_parts_remain.push_back(parts_remain[0]);
        }

        data.ouput_json["partstoupload"] = tmp_parts_remain;

    }

}

void of::logic::process_upload(of::json_processing_data &data) {
    int action = data.input_json["action"];
    switch (action % 10) {
        case 1:
            add_new_file(data);
            break;
        case 2:
            process_chunk_request(data);
            break;
        case 3:
            process_secret_request(data);
            break;
    }
}

void of::logic::glue_file(const std::string token) {
    of::file_info file_inf;
    file_inf.token = token;

    of::db tmp_db;
    tmp_db.set_db_ptr(master_db_obj.get_db_ptr());

    tmp_db.get_active_file_info(file_inf);

    bool last_part_used = false;
    size_t last_part_size = 0;
    size_t parts_temp = file_inf.partnums;
    if (file_inf.partnums * file_inf.partsize < file_inf.filesize) {
        parts_temp = file_inf.partnums + 1;
        last_part_size = file_inf.filesize - file_inf.partnums * file_inf.partsize;
        last_part_used = true;
    }
    for (size_t i = 0; i < parts_temp; ++i) {
        file_inf.partremain.push_back(i);
    }

    std::ofstream output_file;
    output_file.open(info_.storage_directory + "/" + file_inf.hash +"_" + file_inf.token, std::ios::binary);
    std::vector<char> buffer_str;

    for (auto iter = file_inf.partremain.begin(); iter < file_inf.partremain.end(); ++iter) {
        std::ifstream input_file;
        input_file.open(info_.tmp_directory + "/" + file_inf.token + "/" + std::to_string((*iter)) + ".part");

        size_t needed_part_size = 0;
        if ((*iter) == file_inf.partremain.size() - 1 && last_part_used) {
            needed_part_size = last_part_size;
        } else {
            needed_part_size = file_inf.partsize;
        }

        buffer_str.resize(needed_part_size);
        input_file.read(buffer_str.data(), needed_part_size);
        if (input_file.fail()) {
            logging_error("Corrupted part file: " + file_inf.token + "/" + std::to_string(*iter));
            return;
        }

        output_file.write(buffer_str.data(), needed_part_size);
        if(output_file.fail()){
            logging_error("Error writing output file " + file_inf.hash);
            return;
        }

    }

    if (output_file.tellp() != file_inf.filesize) {
        logging_error("Error output file != sum of part sizes " + file_inf.hash);
        remove_stored_file(file_inf);
        return;
    }
    output_file.close();

    md5wrapper md5;
    std::string hash1 = md5.getHashFromFile(info_.storage_directory + "/" + file_inf.hash + "_" +file_inf.token);

    if(hash1 != file_inf.hash){
        logging_error("Hashes mismatched " + file_inf.hash);
        remove_stored_file(file_inf);
    }

    if(remove_directory(info_.tmp_directory + "/" + file_inf.token) != 0){
        logging_error("Error while deleting tmp dir " + file_inf.token);
        return;
    }

    tmp_db.get_active_file_info(file_inf, true);

    file_secret secrets;

    tmp_db.get_stored_file_info(file_inf.token, secrets);

    create_symlink(file_inf, secrets);
}

void of::logic::process_secret_request(of::json_processing_data& data) {
    std::string token = data.input_json["token"];
    file_secret secret_inf;

    data.db_to_operate.get_stored_file_info(token, secret_inf);

    if(secret_inf.id == -1){
        data.ouput_json["status"] = 3;
        return;
    }

    data.ouput_json["file_number"] = std::to_string(secret_inf.id);
    data.ouput_json["secret"] = secret_inf.secret;
    data.ouput_json["status"] = 0;
}


void of::logic::process_download(of::json_processing_data &data) {
    int action = data.input_json["action"];
    switch (action % 10) {
        case 1:
            get_file_info_dw(data);
            break;
        case 2:
            add_file_dw(data);
            break;
        case 3:
            process_chunk_download(data);
            break;
    }
}

void of::logic::get_file_info_dw(of::json_processing_data &data) {
    file_info file_inf;
    file_secret secrets;
    std::string buffer;
    buffer = data.input_json["file_number"];
    secrets.id = std::stoul(buffer);
    secrets.secret = data.input_json["secret"];

    data.db_to_operate.get_stored_file_info_by_secret(file_inf, secrets);

    if(file_inf.filesize == 0){
        data.ouput_json["status"] = 4;
        return;
    }

    data.ouput_json["status"] = 0;
    data.ouput_json["file_name"] = file_inf.filename;
    data.ouput_json["file_size"] = file_inf.filesize;
    data.ouput_json["uniq"] = file_inf.hash;
}

void of::logic::add_file_dw(of::json_processing_data &data) {
    file_info file_inf;
    file_secret secrets;
    std::string buffer;
    file_inf.active_type = DOWNLOAD;
    buffer = data.input_json["file_number"];
    secrets.id = std::stoul(buffer);
    secrets.secret = data.input_json["secret"];

    data.db_to_operate.get_stored_file_info_by_secret(file_inf, secrets);

    size_t received_file_size = data.input_json["file_size"];

    if(file_inf.filesize == 0 || file_inf.filesize != received_file_size){
        data.ouput_json["status"] = 4;
        return;
    }

    of::file_info stored_file_info = file_inf;

    file_inf.partnums = data.input_json["part_num"];
    file_inf.partsize = data.input_json["part_size"];
    file_inf.active_type = DOWNLOAD;
    data.db_to_operate.add_active_file(file_inf);

    if(file_inf.partremain[0] == 0){
        unglue_file(file_inf,stored_file_info);
    }

    if(file_inf.active_type == ERROR){
        data.ouput_json["status"] = 4;
        remove_directory(info_.storage_directory+"/"+file_inf.token);
        data.db_to_operate.get_active_file_info(file_inf, true);
        logging_error("Error while unglueing file " + file_inf.token);
        return;
    }
    data.ouput_json["token"] = file_inf.token;

    std::vector<size_t> tmp_parts_remain;
    tmp_parts_remain.clear();
    if(!file_inf.partremain.empty()){
        tmp_parts_remain.push_back(file_inf.partremain[0]);
    }

    data.ouput_json["partstoupload"] = tmp_parts_remain;
    data.ouput_json["status"] = 0;

}

void of::logic::unglue_file(of::file_info& inf, const of::file_info& stored_file_info) {
    std::ifstream input_file;
    input_file.open(info_.storage_directory+"/"+stored_file_info.hash+"_"+stored_file_info.token, std::ios_base::binary);
    if(!input_file){
        inf.active_type = ERROR;
        logging_error("Error while openning file to download " + inf.token);
        return;
    }

    input_file.seekg(0, std::ios_base::end);
    size_t file_size_disk = input_file.tellg();
    input_file.seekg(0, std::ios_base::beg);
    if(file_size_disk != inf.filesize){
        inf.active_type = ERROR;
        logging_error("Error file wanted to download size uncorrect, file corrupted?  " + inf.token);
        return;
    }

    if(create_directory(info_.tmp_directory+"/"+inf.token) != 0){
        inf.active_type = ERROR;
        logging_error("Error while creating tmp directory " + inf.token);
        return;
    }

    bool last_part_used = false;
    inf.partremain.clear();
    size_t last_part_size = 0;
    size_t parts_temp = inf.partnums;
    if (inf.partnums * inf.partsize < inf.filesize) {
        parts_temp = inf.partnums + 1;
        last_part_size = inf.filesize - inf.partnums * inf.partsize;
        last_part_used = true;
    }
    for (size_t i = 0; i < parts_temp; ++i) {
        inf.partremain.push_back(i);
    }

    std::vector<char> buffer;
    buffer.resize(inf.partsize);

    for(size_t i = 0;i < inf.partremain.size(); ++i){
        std::ofstream output_file;
        size_t current_part_size = 0;
        output_file.open(info_.tmp_directory+"/"+inf.token+"/"+std::to_string(i)+".part", std::ios_base::binary);
        if((i == inf.partremain.size() -1) && last_part_used){
            current_part_size = last_part_size;
        } else{
            current_part_size = inf.partsize;
        }

        input_file.read(buffer.data(), current_part_size);

        if(!input_file){
            inf.active_type = ERROR;
            return;
        }

        output_file.write(buffer.data(), current_part_size);
        if(!output_file){
            inf.active_type = ERROR;
            logging_error("Error writing part file " + inf.token);
            return;
        }

        output_file.close();
    }

}

void of::logic::process_chunk_download(of::json_processing_data &data) {
    of::part_info part_inf;
    part_inf.token = data.input_json["token"];
    part_inf.partcount = data.input_json["part_num"];
    part_inf.active_type = DOWNLOAD;

    data.db_to_operate.get_or_set_file_part(part_inf, false);

    if(part_inf.partsize == 0 || part_inf.active_type != DOWNLOAD){
        data.ouput_json["status"] = 2;
//        logging_error("Error invalid part action " + part_inf.token);
//        data.error = true;
//        data.error_msg = "Token not for this file";
        return;
    }

    std::ifstream input_file;
    input_file.open(info_.tmp_directory+"/"+part_inf.token+"/"+std::to_string(part_inf.partcount)+".part");
    if(!input_file){
        data.ouput_json["status"] = 4;
        logging_error("Error openning part file " + part_inf.token + std::to_string(part_inf.partcount));
        data.error = true;
        data.error_msg = "Error openning part file internal server error";
        return;
    }

    std::vector<char> buffer;
    buffer.resize(part_inf.partsize);

    input_file.read(buffer.data(), part_inf.partsize);

    if(!input_file){
        data.ouput_json["status"] = 4;
        logging_error("Error part file corrupted " + part_inf.token + std::to_string(part_inf.partcount));
        data.error = true;
        data.error_msg = "Error part file corrupted internal server error";
        input_file.close();
        return;
    }

    input_file.close();

    data.ouput_str.append(buffer.data(), buffer.size());

    data.ouput_json["status"] = 0;

    data.db_to_operate.get_or_set_file_part(part_inf, true);
    std::vector<size_t> parts_remain;

    data.db_to_operate.get_remain_parts_of_file(part_inf.token, parts_remain);

   std::vector<size_t> tmp_parts_remain;
   tmp_parts_remain.clear();
   if(!parts_remain.empty()){
       tmp_parts_remain.push_back(parts_remain[0]);
   }

    data.ouput_json["partstoupload"] = tmp_parts_remain;
    data.ouput_json["part_size"] = part_inf.partsize;

    if(parts_remain.empty()){
        clear_finished_active_files();
    }
}

void of::logic::clear_finished_active_files() {
    of::db tmp_db;
    tmp_db.set_db_ptr(master_db_obj.get_db_ptr());

    std::vector<std::string> deleted_tokens;
    tmp_db.clear_active_files(deleted_tokens);

    for(auto iter = deleted_tokens.begin(); iter < deleted_tokens.end(); ++iter){
        if(remove_directory(info_.tmp_directory+"/"+(*iter)) != 0){
            logging_error("Error while clearing directories of downloaded files " + *iter);
        }
    }
}

void of::logic::remove_stored_file(const of::file_info &inf) {
    of::db tmp_db;
    tmp_db.set_db_ptr(master_db_obj.get_db_ptr());

    tmp_db.remove_stored_file(inf);
    if(remove_directory(info_.storage_directory + "/" + inf.hash + "_" + inf.token) != 0){
        logging_error("Error deleting hash mismached file " + inf.hash + "_" + inf.token);
    }

}

void of::logic::add_not_downloaded_part(of::json_processing_data &data) {
    try{
        if(data.input_json["action"] == 23){
            part_info part;
            part.token = data.input_json["token"];
            part.partcount = data.input_json["part_num"];
            data.db_to_operate.add_not_uploaded_part(part);
        }
    } catch (std::exception& e){
        logging_error("Error while adding not downloaded part");
    }
}