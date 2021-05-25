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
    boost::filesystem::permissions(dir_name, boost::filesystem::owner_all, code);
    if (code) {
        logging_error("ERROR WHILE CHANGING PERMISSION! CHANGE OWNER OF " + dir_name + " DIR");
        return -1;
    }
    return 0;
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

        data.ouput_json["partstoupload"] = remain_parts;

        data.glue_needing = false;
        if(remain_parts.empty()){
            data.glue_needing = true;
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
        }

        data.ouput_str = to_string(data.ouput_json);
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

    data.db_to_operate.add_active_file(input_file);

    if (create_directory(info_.tmp_directory + "/" + input_file.token) != 0) {
        data.error = true;
        data.error_msg = "Error creating directory to part files";
        logging_error(data.error_msg + " "+ input_file.token);
    }

    data.ouput_json["token"] = input_file.token;
    data.ouput_json["partstoupload"] = input_file.partremain;
    data.ouput_json["status"] = 0;

}

void of::logic::process_chunk_request(of::json_processing_data &data) {
    of::part_info part_file;
    part_file.token = data.input_json["token"];
    part_file.partcount = data.input_json["part_num"];

    data.db_to_operate.get_or_set_file_part(part_file, false);

    if (part_file.partsize > 0) {
        data.bytes_waiting = part_file.partsize;
    } else {
        data.ouput_json["status"] = 2;
        std::vector<size_t> parts_remain;
        data.db_to_operate.get_remain_parts_of_file(part_file.token, parts_remain);
        data.ouput_json["partstoupload"] = parts_remain;
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
    output_file.open(info_.storage_directory + "/" + file_inf.hash, std::ios::binary);
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
        return;
    }
    output_file.close();

//    md5wrapper md5;
//    std::string hash1 = md5.getHashFromFile(info_.storage_directory + "/" + file_inf.hash);
//
//    if(hash1 != file_inf.hash){
//        logging_error("Hashes mismatched " + file_inf.hash);
//        return;
//    }

    if(remove_directory(info_.tmp_directory + "/" + file_inf.token) != 0){
        logging_error("Error while deleting tmp dir " + file_inf.token);
        return;
    }

    tmp_db.get_active_file_info(file_inf, true);
    tmp_db.add_stored_file(file_inf);

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
