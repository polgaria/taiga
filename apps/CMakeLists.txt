add_executable(taigabot-main taigabot.cpp)
target_compile_features(taigabot-main PRIVATE cxx_std_17)

target_link_libraries(taigabot-main PRIVATE taigabot)