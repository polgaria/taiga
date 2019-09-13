add_executable(taiga-main taiga.cpp)
target_compile_features(taiga-main PRIVATE cxx_std_17)

target_link_libraries(taiga-main PRIVATE taiga)