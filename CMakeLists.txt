cmake_minimum_required(VERSION 3.12...3.15)

project(taiga VERSION 1.2
              DESCRIPTION "discord bot"
              LANGUAGES CXX)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)    
    set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)	

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")
endif()

add_subdirectory(lib/aisaka)
find_package(aisaka		 REQUIRED)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx  REQUIRED)
find_package(Aegis		 REQUIRED)
find_package(fifo_map	 REQUIRED)

add_subdirectory(lib/date)
add_subdirectory(lib/cpr)

add_subdirectory(src)
add_subdirectory(apps)