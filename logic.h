#ifndef BOOST_TCP_LOGIC_H
#define BOOST_TCP_LOGIC_H
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <fstream>
#include "json/single_include/nlohmann/json.hpp"
#include "db.h"


namespace of{
    typedef struct {
        std::string input_str;
        std::string ouput_str;
        nlohmann::json input_json;
        nlohmann::json ouput_json;
        std::string error_msg;
        bool error;
        bool glue_needing;
        size_t bytes_waiting;
        of::db db_to_operate;
    } json_processing_data;

    typedef struct{
        std::string db_file;
        std::string tmp_directory;
        std::string storage_directory;
        std::string log_file;
        std::string nginx_directory;
    } server_logic_info;



    const server_logic_info default_server_logic_info = {
            "of_db.sqlite3", "tmp_dir", "storage_dir", "of.log", "nginx"
    };

    class logic{
    public:
        logic(const of::server_logic_info& info = default_server_logic_info);
        logic(logic& rhs) = default;
        ~logic();
        sqlite3* get_master_db_ptr();

        void process_json(of::json_processing_data& data);

        void process_upload(of::json_processing_data& data);
        void process_download(of::json_processing_data& data);

        void get_file_info_dw(of::json_processing_data& data);
        void add_file_dw(of::json_processing_data& data);
        void process_chunk_download(of::json_processing_data& data);

        void add_new_file(of::json_processing_data& data);
        void process_chunk_request(of::json_processing_data& data);
        void process_fetched_chunk(of::json_processing_data& data, std::string& part_file);
        void process_secret_request(of::json_processing_data& data);

        void add_not_downloaded_part(of::json_processing_data& data);

        void glue_file(const std::string token);

    private:
        int create_directory(const std::string& dir_name);
        void create_symlink(file_info& file_inf, file_secret& secret);
        int remove_directory(const std::string& dir_name);
        void remove_stored_file(const of::file_info& inf);
        void clear_finished_active_files();
        void unglue_file(of::file_info& inf, const of::file_info& stored_file_info);

        void logging_error(std::string error_msg);
        std::ofstream log_file;
        of::db master_db_obj;
        const of::server_logic_info& info_;

    };

}

#endif //BOOST_TCP_LOGIC_H
