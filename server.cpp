#include "server.h"


#define BUFFER_LENGHT 1024

using boost::asio::ip::tcp;

of::session::session(boost::asio::io_context &io_context, of::logic &logic_obj)
        : socket_(io_context), logic_(logic_obj), io_service(io_context) {

    json_proc_data.db_to_operate.set_db_ptr(logic_.get_master_db_ptr());
    json_proc_data.glue_needing = false;
    json_proc_data.error = false;
    buffer_str_capacity = BUFFER_LENGHT;
    bufferStr.resize(buffer_str_capacity);
}

tcp::socket &of::session::socket() {
    return socket_;
}

void of::session::start() {
    socket_.set_option(boost::asio::ip::tcp::socket::keep_alive(true));
    socket_.async_read_some(boost::asio::buffer(bufferStr, buffer_str_capacity),
                            boost::bind(&session::handle_read, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void of::session::add_data(size_t bytes_received) {
    size_t j = data_string.length();
    data_string.resize(data_string.length() + bytes_received);
    for (size_t i = 0; i < bytes_received; i++) {
        data_string[j] = bufferStr[i];
        ++j;
    }
}

size_t of::session::count_character(std::string string_to_find, char character) {
    size_t position = 0;
    size_t counter = 0;
    while (1) {
        position = string_to_find.find(character);
        if (position == std::string::npos) {
            break;
        }
        string_to_find = string_to_find.substr(position + 1, string_to_find.length());
        ++counter;
    }
    return counter;
}

bool of::session::check_all_json_readed(std::string &string_to_check) {
    if(find_end_of_json() > 0){
        return true;
    }

//    if (count_character(string_to_check, '{') == count_character(string_to_check, '}')) {
//        return true;
//    }
    return false;
}

void of::session::process_error(of::json_processing_data &json_proc_data) {
    json_proc_data.ouput_str =
            "Denis, y nas exception na barabane\nTvoy zapros\n" + data_string + "\nPolychi exception\n" +
            json_proc_data.error_msg + "\n";
    json_proc_data.error = false;

    boost::asio::async_write(socket_,
                             boost::asio::buffer(json_proc_data.ouput_str,
                                                 json_proc_data.ouput_str.length()),
                             boost::bind(&session::handle_write, this,
                                         boost::asio::placeholders::error));

    data_string.clear();
}

void of::session::handle_file_read(const boost::system::error_code &error,
                                   size_t bytes_transferred) {
    if (!error) {

        add_data(bytes_transferred);

        if (data_string.length() < json_proc_data.bytes_waiting) {
            socket_.async_read_some(boost::asio::buffer(bufferStr, buffer_str_capacity),
                                    boost::bind(&session::handle_file_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
            return;
        }

        logic_.process_fetched_chunk(json_proc_data, data_string);

        if (json_proc_data.error) {
            process_error(json_proc_data);
            return;
        }

        boost::asio::async_write(socket_,
                                 boost::asio::buffer(json_proc_data.ouput_str,
                                                     json_proc_data.ouput_str.length()),
                                 boost::bind(&session::handle_write, this,
                                             boost::asio::placeholders::error));
        data_string.clear();

        if (json_proc_data.glue_needing) {
            of::file_info file_to_glue;
            io_service.post(boost::bind(&of::logic::glue_file, &logic_, json_proc_data.input_json["token"]));
        }

        data_string.clear();

    } else {
        delete this;
    }
}

size_t of::session::find_end_of_json() {
    size_t opening_quotes = 0;
    size_t closing_quotes = 0;
    size_t i = 0;
    bool check_needed = false;
    while (1) {
        if(i >= data_string.length()){
            i = -1;
            break;
        }
        switch (data_string[i]) {
            case '{':
                ++opening_quotes;
                check_needed = true;
                break;
            case '}':
                ++closing_quotes;
                check_needed = true;
                break;
        }
        if (check_needed && closing_quotes == opening_quotes) {
            break;
        }
        ++i;
    }
    ++i;
    return i;
}

void of::session::remove_json_from_data() {
    size_t end = find_end_of_json();
    data_string.erase(0, end);
}

void of::session::handle_read(const boost::system::error_code &error,
                              size_t bytes_transferred) {
    if (!error) {
        add_data(bytes_transferred);

        if (!check_all_json_readed(data_string)) {
            socket_.async_read_some(boost::asio::buffer(bufferStr, buffer_str_capacity),
                                    boost::bind(&session::handle_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
            return;
        }

        json_proc_data.input_str = data_string.substr(0, find_end_of_json());
        json_proc_data.bytes_waiting = 0;

        logic_.process_json(json_proc_data);

        if (!json_proc_data.error && json_proc_data.bytes_waiting > 0) {
            remove_json_from_data();
            handle_file_read(error, 0);
            return;
        }

        if (json_proc_data.error) {
            process_error(json_proc_data);
            return;
        }

        boost::asio::async_write(socket_,
                                 boost::asio::buffer(json_proc_data.ouput_str,
                                                     json_proc_data.ouput_str.length()),
                                 boost::bind(&session::handle_write, this,
                                             boost::asio::placeholders::error));
        //json_proc_data.input_json.clear();
        json_proc_data.input_str.clear();
        data_string.clear();
    } else {
        logic_.add_not_downloaded_part(json_proc_data);
        delete this;
    }
}

void of::session::handle_write(const boost::system::error_code &error) {
    if (!error) {
        json_proc_data.ouput_str.clear();
        json_proc_data.ouput_json.clear();
        socket_.async_read_some(boost::asio::buffer(bufferStr, buffer_str_capacity),
                                boost::bind(&session::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    } else {
        delete this;
    }
}


of::server::server(server_info &info)
        : io_context_(info.service),
          acceptor_(info.service, tcp::endpoint(tcp::v4(), info.port)),
          logic_(info.logic) {
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    start_accept();
}

void of::server::start_accept() {
    session *new_session = new session(io_context_, logic_);
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&server::handle_accept, this, new_session,
                                       boost::asio::placeholders::error));
}

void of::server::handle_accept(session *new_session,
                               const boost::system::error_code &error) {
    if (!error) {
        new_session->start();
    } else {
        delete new_session;
    }

    start_accept();
}

static void thread_job(boost::asio::io_context* service){
    service->run();
}

void of::start_threads_server(server_info &info) {
    of::server serv(info);
    if(info.num_of_threads == 0){
        info.service.run();
        return;
    }
    if(info.num_of_threads == -1){
        info.num_of_threads = boost::thread::hardware_concurrency();
    }

    boost::thread_group threads;


    for(size_t i = 0; i < info.num_of_threads; ++i){
        threads.create_thread(boost::bind(thread_job, &info.service));
    }

    threads.join_all();
}