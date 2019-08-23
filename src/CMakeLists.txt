set(HEADER_LIST "${taigabot_SOURCE_DIR}/include/taigabot/Client.hpp" "${taigabot_SOURCE_DIR}/include/taigabot/Util.hpp") 

add_library(taigabot Client.cpp Util.cpp ${HEADER_LIST})

target_include_directories(taigabot PUBLIC ../include)

target_link_libraries(taigabot sleepy-discord rapidjson)

target_compile_features(taigabot PUBLIC cxx_std_17)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
