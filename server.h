#ifndef BOOST_TCP_SERVER_H
#define BOOST_TCP_SERVER_H
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "logic.h"

namespace of{
    typedef struct {
        boost::asio::io_context& service;
        short port;
        of::server_logic_info logic;
        size_t num_of_threads;
    } server_info;

    using boost::asio::ip::tcp;
    class session {
    public:
        session(boost::asio::io_context& io_context, of::logic& logic_obj);
        tcp::socket& socket();
        void start();
    private:
        size_t count_character(std::string string_to_find, char character);
        bool check_all_json_readed(std::string& string_to_check);
        void process_error(of::json_processing_data& json_proc_data);
        void handle_file_read(const boost::system::error_code& error, size_t bytes_transferred);
        size_t find_end_of_json();
        void remove_json_from_data();
        void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
        void handle_write(const boost::system::error_code& error);

        void add_data(size_t bytes_received);

        of::logic& logic_;
        of::json_processing_data json_proc_data;
        size_t needed_bytes_to_read;
        tcp::socket socket_;
        std::string data_string;
        std::string bufferStr;
        size_t buffer_str_capacity;
        boost::asio::io_context& io_service;
    };

    class server {
    public:
        server(server_info &info);
    private:
        void start_accept();
        void handle_accept(session* new_session, const boost::system::error_code& error);

        of::logic logic_;
        boost::asio::io_context& io_context_;
        tcp::acceptor acceptor_;
    };

    void start_threads_server(server_info &info);
}



#endif //BOOST_TCP_SERVER_H
