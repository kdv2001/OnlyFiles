cmake_minimum_required(VERSION 3.16.2)
project(OnlyFiles)

set(CMAKE_CXX_STANDARD 17)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(helloworld JSon_render.h JSon_render.cpp main.cpp)
target_link_libraries(OnlyFiles Qt5::Widgets)