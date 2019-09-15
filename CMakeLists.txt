cmake_minimum_required(VERSION 3.12...3.15)

project(taiga     VERSION 1.0
                  DESCRIPTION "discord bot"
                  LANGUAGES CXX)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)    
    set(CMAKE_CXX_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

find_package(PkgConfig)
find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
find_package(Aegis REQUIRED)

add_subdirectory(lib/date)
add_subdirectory(lib/cpr)

add_library(fifo_map INTERFACE)
target_include_directories(fifo_map INTERFACE ./lib/fifo_map/src)

add_subdirectory(src)
add_subdirectory(apps)

