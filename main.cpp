#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "server.h"


int main(int argc, char* argv[]){

    try{
        boost::asio::io_context io_service;
        of::server_info server_info_data = {io_service, 8080, of::default_server_logic_info, static_cast<size_t>(-1)};

        of::start_threads_server(server_info_data);

    } catch (std::exception& exception) {
        std::cout << exception.what();
    }

    return 0;
}