cmake_minimum_required(VERSION 3.16.2)
project(OnlyFiles)

set(CMAKE_CXX_STANDARD 17)

add_executable(OnlyFiles main.cpp client/JSon_renderer.cpp client/JSon_renderer.h)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

target_link_libraries(OnlyFiles Qt5::Widgets)

