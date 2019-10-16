cmake_minimum_required(VERSION 3.12...3.15)

project(taiga VERSION 1.2
              DESCRIPTION "discord bot"
              LANGUAGES CXX)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)    
    set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")
endif()

add_subdirectory(lib/aisaka)
add_subdirectory(lib/date)
add_subdirectory(lib/cpr)

add_subdirectory(src)
add_subdirectory(apps)
