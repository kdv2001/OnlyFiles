cmake_minimum_required(VERSION 3.16.3)
project(OnlyFiles)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS UI_files)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Network Core Gui Widgets REQUIRED)
find_package(Qt5Network)

enable_testing()

include_directories(include)

set(MOC_HEADERS
        client/main_window.h
        client/JSon_renderer.h
        client/download_window.h
        )

set(UIS
        main_window.ui
        download_window.ui
        download_window_2.ui
        staked_windows.ui
        untitled.ui)

add_executable(
        OnlyFiles

        client/main.cpp
        client/download_window.cpp
        client/main_window.cpp
        client/JSon_renderer.cpp

        ${MOC_HEADERS}

        ${UI_HEADERS}
)


#add_subdirectory(tests)
#add_subdirectory(qtTesting)

target_link_libraries(OnlyFiles Qt5::Widgets Qt5::Core Qt5::Network Qt5::Gui)
target_link_libraries(OnlyFiles Qt5::Network)