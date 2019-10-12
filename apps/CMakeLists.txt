add_executable(taiga-main taiga.cpp)

target_compile_features(taiga-main PRIVATE cxx_std_17 ${Aegis_CFLAGS_OTHER})
if(MSVC)
	target_compile_options(taiga-main PRIVATE $<$<CONFIG:RELEASE>:/Ox> /W4)
else()
	target_compile_options(taiga-main PRIVATE $<$<CONFIG:RELEASE>:-O3> -Wall -Wextra -pedantic)
endif()

target_link_libraries(taiga-main PUBLIC taiga)

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
	set_target_properties(taiga-main PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()