#ifndef ONLY_FILES_DB_H
#define ONLY_FILES_DB_H

#include <string>
#include <vector>
#include <map>
#include "sqlite/sqlite3.h"

namespace of {
    typedef struct{
        size_t id;
        std::string secret;
    }file_secret;

    typedef struct{
        std::string filename;
        std::string hash;
        size_t filesize;
        size_t partsize;
        size_t partnums;

        std::string token;
        std::vector<size_t> partremain;
    }file_info;

    typedef struct{
        size_t partsize;
        size_t partcount;
        std::string token;
    }part_info;

    class db {
    public:
        db();
//        db(sqlite3* db_ptr);
        db(db& rhs) = default;
        ~db();
        void open_db(const std::string &filename);
        void close_db();
        sqlite3* get_db_ptr();
        void set_db_ptr(sqlite3* db_ptr);

        void add_active_file(file_info& file);
        void add_stored_file(file_info& file);
        void get_active_file_info(file_info& file, bool remove = false);
        void get_stored_file_info(const std::string& token, file_secret& secret);
        void get_or_set_file_part(part_info& part, bool update_db);
        void get_file_hash(const std::string& token, std::string& file_hash);
        void get_remain_parts_of_file(const std::string& token, std::vector<size_t>& part_remain);

        //void get_file_part_info(part_info& part);

//        void move_to_stored_file(const std::string& token, file_secret& secret);



    private:
        sqlite3 *db_;
        std::string query_;
        std::vector<std::map<std::string, std::string>> select_res_;
        std::string format(const char *fmt, ...);

        friend int select_res_callback(void *arg, int argc, char **key, char **value);

        void init_db();
        void execute();
        std::string size_to_str(size_t input);
        std::string generate_random_number(size_t);

        std::vector<size_t> get_remain_parts(std::string);
        std::string get_remain_parts_r(std::vector<size_t>&);
    };


class db_exceptions : public std::exception{
    const char * what() const noexcept{return error_msg.c_str();}
public:
    db_exceptions(const char *message);
    std::string error_msg;
};
}

#endif //ONLY_FILES_DB_H
