#include <sstream>
#include <cstdlib>
#include <ctime>
#include "db.h"


namespace of {
    int select_res_callback(void *arg, int argc, char **value, char **key) {
        auto object = static_cast<class of::db *>(arg);
        std::map<std::string, std::string> elem_to_push;
        for(int i = 0; i < argc; ++i){
            elem_to_push[key[i]] = value[i];
        }
        object->select_res_.push_back(elem_to_push);
        return 0;
    }
}

std::string of::db::format(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::vector<char> v(1024);
    while (true)
    {
        va_list args2;
        va_copy(args2, args);
        int res = vsnprintf(v.data(), v.size(), fmt, args2);
        if ((res >= 0) && (res < static_cast<int>(v.size())))
        {
            va_end(args);
            va_end(args2);
            return std::string(v.data());
        }
        size_t size;
        if (res < 0)
            size = v.size() * 2;
        else
            size = static_cast<size_t>(res) + 1;
        v.clear();
        v.resize(size);
        va_end(args2);
    }
}

std::string of::db::size_to_str(size_t input) {
    return std::to_string(input);
}

std::string of::db::generate_random_number(size_t count) {
    std::string output;
    for(size_t i = 0; i < count; ++i){
        output.append(size_to_str(rand()%10));
    }
    return output;
}

of::db_exceptions::db_exceptions(const char *message) {
    error_msg = message;
}

of::db::db() {
//    open_db(db_filename);
}

//of::db::db(sqlite3 *db_ptr) : db_(db_ptr) {
//
//}

void of::db::set_db_ptr(sqlite3 *db_ptr) {
    db_ = db_ptr;
}

of::db::~db() {
}

void of::db::close_db() {
    sqlite3_close(db_);
}

sqlite3 * of::db::get_db_ptr() {
    return db_;
}

void of::db::execute() {
    int exit_status = 0;
    char* error_char = NULL;
    if(query_.find("SELECT") == std::string::npos){
        exit_status = sqlite3_exec(db_, query_.c_str(), NULL, NULL, &error_char);
    } else{
        select_res_.clear();
        exit_status = sqlite3_exec(db_, query_.c_str(), select_res_callback, this, &error_char);
    }
    if(exit_status != SQLITE_OK){
        of::db_exceptions exc(error_char);
        sqlite3_free(error_char);
        throw exc;
    }

}

void of::db::init_db() {
    query_ = "CREATE TABLE stored_files(id INTEGER PRIMARY KEY, filename TEXT, filesize INTEGER, secret TEXT, md5 TEXT, token TEXT, time TEXT);"
             "CREATE TABLE active_files(md5 TEXT, filename TEXT, filesize INTEGER, partnums INTEGER, partsize INTEGER, token TEXT, partremain TEXT, time TEXT, download INTEGER);";
    execute();
}

void of::db::open_db(const std::string &filename) {
    int exit_status = sqlite3_open(filename.c_str(), &db_);
    if(exit_status != SQLITE_OK){
        throw of::db_exceptions("Openning DB error");
    }
    srand(time(NULL));

    query_ = "SELECT name FROM sqlite_master WHERE type='table'";
    execute();
    if(select_res_.empty()){
        init_db();
    }
}

std::vector<size_t> of::db::get_remain_parts(std::string input) {
    std::vector<size_t> parts;
    while(1){
        size_t position = input.find(' ');
        if(position == std::string::npos){
            try{
                parts.push_back(std::stoul(input.substr(0, input.length())));
            }catch (std::invalid_argument) {

            }
            break;
        }
        try{
            parts.push_back(std::stoul(input.substr(0, position)));
        }catch (std::invalid_argument) {

        }
        input = input.substr(position+1, input.length()-1);
    }
    return parts;
}

std::string of::db::get_remain_parts_r(std::vector<size_t> &input) {
    std::string out_str;
    for(size_t i = 0; i < input.size(); ++i){
        out_str = out_str + size_to_str(input[i]);
        if(i < input.size()-1){
            out_str = out_str + " ";
        }
    }
    return out_str;
}

void of::db::add_active_file(of::file_info& info) {
    query_ = format("SELECT * FROM active_files WHERE md5='%s' AND filesize=%d AND partsize=%d AND download=%d;", info.hash.c_str(), info.filesize, info.partsize, info.active_type);
    execute();
    if(!select_res_.empty()){
        info.partnums = std::stoul(select_res_[0]["partnums"]);
        info.partsize = std::stoul(select_res_[0]["partsize"]);
        info.partremain = get_remain_parts(select_res_[0]["partremain"]);
        info.token = select_res_[0]["token"];
        return;
    }

    std::string token;
    do{
        token = generate_random_number(8);
        query_ = format("SELECT * FROM stored_files WHERE token='%s'", token.c_str());
        execute();
        if(!select_res_.empty()){
            continue;
        }

        query_ = format("SELECT * FROM active_files WHERE token='%s'", token.c_str());
        execute();
    } while (!select_res_.empty());


    size_t parts_temp = info.partnums;
    if(info.partnums*info.partsize < info.filesize){
        parts_temp = info.partnums + 1;
    }
    for(size_t i = 0; i < parts_temp; ++i){
        info.partremain.push_back(i);
    }

    auto current_time = time(NULL);

    query_ = format("INSERT INTO active_files(md5, filename, filesize, partnums, partsize, token, partremain, time, download) VALUES ('%s', '%s', %d, %d, %d, '%s', '%s', '%s', '%s')",
                    info.hash.c_str(), info.filename.c_str(), info.filesize, info.partnums, info.partsize, token.c_str(),
                    get_remain_parts_r(info.partremain).c_str(), size_to_str(current_time).c_str(), size_to_str(info.active_type).c_str());
    execute();
    info.token = token;

}

//void of::db::get_file_part_info(part_info &part) {
//    query_ = format("SELECT * FROM active_files WHERE token='%s'", part.token.c_str());
//    execute();
//    std::vector<size_t> parts_db = get_remain_parts(select_res_[0]["partremain"]);
//
//    bool erased = false;
//    for(auto iterator = parts_db.begin(); iterator < parts_db.end(); ++iterator){
//        if(*iterator == part.partcount){
//            parts_db.erase(iterator);
//            erased = true;
//            break;
//        }
//    }
//
//    if(!erased){
//        part.partsize = 0;
//        return;
//    }
//
//    if(part.partcount == std::stoul(select_res_[0]["partnums"])){
//        size_t part_size = std::stoul(select_res_[0]["partsize"]);
//        size_t part_nums = std::stoul(select_res_[0]["partnums"]);
//        size_t file_size = std::stoul(select_res_[0]["filesize"]);
//
//        part.partsize = file_size - part_nums*part_size;
//    }else{
//        part.partsize = std::stoul(select_res_[0]["partsize"]);
//    }
//
//}

void of::db::get_or_set_file_part(part_info &part, bool update_db) {
    query_ = format("SELECT * FROM active_files WHERE token='%s'", part.token.c_str());
    execute();
    if(select_res_.empty()){
        part.active_type = ERROR;
        part.partsize = 0;
        return;
    }
    if(part.active_type != std::stoul(select_res_[0]["download"])){
        part.partsize = 0;
        part.active_type = ERROR;
        return;
    }

    std::vector<size_t> parts_db = get_remain_parts(select_res_[0]["partremain"]);

    bool erased = false;
    for(auto iterator = parts_db.begin(); iterator < parts_db.end(); ++iterator){
        if(*iterator == part.partcount){
            parts_db.erase(iterator);
            erased = true;
            break;
        }
    }
    if(!erased){
        part.partsize = 0;
        part.active_type = ERROR;
        return;
    }

    if(part.partcount == std::stoul(select_res_[0]["partnums"])){
        size_t part_size = std::stoul(select_res_[0]["partsize"]);
        size_t part_nums = std::stoul(select_res_[0]["partnums"]);
        size_t file_size = std::stoul(select_res_[0]["filesize"]);

        part.partsize = file_size - part_nums*part_size;
    }else{
        part.partsize = std::stoul(select_res_[0]["partsize"]);
    }

    if(update_db){
        query_ = format("UPDATE active_files SET partremain = '%s' WHERE token = '%s'", get_remain_parts_r(parts_db).c_str(),
                        part.token.c_str());
        execute();
    }
}

//void of::db::move_to_stored_file(const std::string& token, file_secret &secret) {
//    query_ = format("SELECT * FROM active_files WHERE token = '%s'", token.c_str());
//    execute();
//    if(select_res_.empty()){
//        secret.id = 0;
//        secret.secret = "";
//        return;
//    }
//    std::string& filename = select_res_[0]["filename"];
//    size_t filesize = std::stoul(select_res_[0]["filesize"]);
//    std::string& hash = select_res_[0]["md5"];
//
//    secret.secret = generate_random_number(8);
//
//    query_ = format("INSERT INTO stored_files(filename, filesize, secret, md5, token) VALUES ('%s', %lu, '%s', '%s', '%s')",
//                    filename.c_str(), filesize, secret.secret.c_str(), hash.c_str(), token.c_str());
//    execute();
////    query_ = format("SELECT id FROM stored_files WHERE token = '%s'", token.c_str());
////    execute();
////    secret.id = std::stoll(select_res_[0]["id"]);
//
//    query_ = format("DELETE FROM active_files WHERE token = '%s'", token.c_str());
//    execute();
//}

void of::db::get_file_hash(const std::string &token, std::string& file_hash) {
    query_ = format("SELECT md5 FROM active_files WHERE token='%s';", token.c_str());
    execute();
    try{
        file_hash =  select_res_[0]["md5"];
    } catch (std::exception) {
        file_hash =  "removed";
    }
}

void of::db::get_remain_parts_of_file(const std::string &token, std::vector<size_t> &part_remain) {
    query_ = format("SELECT partremain FROM active_files WHERE token = '%s'", token.c_str());
    execute();
    if(select_res_.empty()){
        part_remain.clear();
        part_remain.push_back(-1);
        return;
    }

    part_remain = get_remain_parts(select_res_[0]["partremain"]);

}

void of::db::get_active_file_info(file_info &file, bool remove) {
    query_ = format("SELECT * FROM active_files WHERE token = '%s'", file.token.c_str());
    execute();

    file.partsize = std::stoul(select_res_[0]["partsize"]);
    file.partnums = std::stoul(select_res_[0]["partnums"]);
    file.filesize = std::stoul(select_res_[0]["filesize"]);
    file.hash = select_res_[0]["md5"];
    file.filename = select_res_[0]["filename"];

    if(remove){
        query_ = format("DELETE FROM active_files WHERE token = '%s'", file.token.c_str());
        execute();
    }
}


void of::db::add_stored_file(file_info &file) {
    const std::string& secret = generate_random_number(8);
    auto current_time = time(NULL);

    query_ = format("INSERT INTO stored_files(filename, filesize, secret, md5, token, time) VALUES ('%s', %lu, '%s', '%s', '%s', %s)",
                    file.filename.c_str(), file.filesize, secret.c_str(), file.hash.c_str(), file.token.c_str(),
                    size_to_str(current_time).c_str());
    execute();
}

void of::db::get_stored_file_info(const std::string &token, file_secret& secret) {
    query_ = format("SELECT id, secret, time FROM stored_files WHERE token = '%s'", token.c_str());
    execute();
    if(select_res_.empty()){
        secret.id = -1;
        return;
    }

    size_t time_now = time(NULL);
    size_t time_added = std::stoul(select_res_[0]["time"]);

    if(time_now - time_added > TIME_TO_GET_FILE_SECRETS_IN_SECS){
        secret.id = -1;
        return;
    }

    secret.id = std::stoul(select_res_[0]["id"]);
    secret.secret = select_res_[0]["secret"];
}

void of::db::remove_stored_file(const file_info& file) {
    query_ = format("DELETE FROM stored_files WHERE token='%s' AND md5='%s'", file.token.c_str(), file.hash.c_str());
    execute();
}

void of::db::get_stored_file_info_by_secret(file_info &file_inf, const file_secret &secret) {
    query_ = format("SELECT * FROM stored_files WHERE id = %lu AND secret = '%s'", secret.id, secret.secret.c_str());
    execute();

    if(select_res_.empty()){
        file_inf.filesize = 0;
        return;
    }

    file_inf.filename = select_res_[0]["filename"];
    file_inf.filesize = std::stoul(select_res_[0]["filesize"]);
    file_inf.hash = select_res_[0]["md5"];
    file_inf.token = select_res_[0]["token"];

}

void of::db::clear_active_files(std::vector<std::string>& deleted_tokens) {
    query_ = format("SELECT * FROM active_files");
    execute();

    for(size_t i = 0; i < select_res_.size(); ++i){
        if(select_res_[i]["partremain"].empty() && std::stoul(select_res_[i]["download"]) == DOWNLOAD){
            query_ = format("DELETE FROM active_files WHERE token = '%s'", select_res_[i]["token"].c_str());
            execute();
            deleted_tokens.push_back(select_res_[i]["token"]);
        }
    }
}

void of::db::add_not_uploaded_part(const part_info &inf) {
    query_ = format("SELECT * FROM active_files WHERE token='%s'", inf.token.c_str());
    execute();
    std::vector<size_t> parts_uploaded = get_remain_parts(select_res_[0]["partremain"]);
    for(size_t i = 0; i < 2; ++i){
        if(inf.partcount - i < 500){
            parts_uploaded.push_back(inf.partcount - i);
        }
    }
    std::string parts_modified = get_remain_parts_r(parts_uploaded);
    query_ = format("UPDATE active_files SET partremain = '%s' WHERE token='%s'", parts_modified.c_str(), inf.token.c_str());
    execute();
}