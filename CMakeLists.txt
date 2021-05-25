cmake_minimum_required(VERSION 3.19)
project(boost_tcp)

set(CMAKE_CXX_STANDARD 14)
set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost COMPONENTS system filesystem thread)
add_library(sqlite3 sqlite/sqlite3.c sqlite/sqlite3.h)
add_library(hashmd5 hash_md5/md5.cpp hash_md5/md5.h hash_md5/md5wrapper.cpp hash_md5/md5wrapper.h)
find_library(dl dl)

add_subdirectory(json)

add_executable(boost_tcp main.cpp db.cpp db.h logic.cpp logic.h server.cpp server.h)
target_link_libraries(boost_tcp ${Boost_LIBRARIES} pthread sqlite3 dl nlohmann_json::nlohmann_json hashmd5)