cmake_minimum_required(VERSION 3.12...3.15)

project(taigabot VERSION 1.0
                  DESCRIPTION "discord bot"
                  LANGUAGES CXX)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)    
    set(CMAKE_CXX_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

add_subdirectory(src)

add_subdirectory(apps)

add_subdirectory(sleepy-discord)

add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE ./rapidjson/include)

