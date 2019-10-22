cmake_minimum_required(VERSION 3.12...3.15)

set(taiga_VERSION_MAJOR 1)
set(taiga_VERSION_MINOR 5)
set(taiga_VERSION_PATCH 1)
set(taiga_VERSION "${taiga_VERSION_MAJOR}.${taiga_VERSION_MINOR}.${taiga_VERSION_PATCH}")

project(
  taiga
  VERSION ${taiga_VERSION}
  DESCRIPTION "discord bot"
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")
endif()

set(BUILD_TZ_LIB
    ON
    CACHE INTERNAL "")

find_package(libmongocxx REQUIRED)

add_subdirectory(lib/aisaka)
add_subdirectory(lib/date)
add_subdirectory(lib/cpr)

add_subdirectory(src)
add_subdirectory(apps)
