set(HEADER_LIST
	"${taiga_SOURCE_DIR}/include/taiga/categories/General.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/categories/Prefix.hpp"	
	"${taiga_SOURCE_DIR}/include/taiga/categories/Reddit.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/categories/Conversion.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/categories/Timezone.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/categories/Miscellaneous.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/categories/Weather.hpp"	
	"${taiga_SOURCE_DIR}/include/taiga/Config.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/util/Various.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/util/Math.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/util/String.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/util/Command.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/Client.hpp"
)

add_library(taiga
	util/Command.cpp
	util/Various.cpp
	util/String.cpp
	categories/General.cpp
	categories/Prefix.cpp
	categories/Reddit.cpp
	categories/Conversion.cpp
	categories/Timezone.cpp
	categories/Miscellaneous.cpp
	categories/Weather.cpp
	Client.cpp
	${HEADER_LIST}
)

target_include_directories(taiga PUBLIC
	../include
	${CPR_INCLUDE_DIRS}
	${LIBMONGOCXX_INCLUDE_DIRS}
)

target_link_libraries(taiga PUBLIC
	aisaka::aisaka
	date
	tz
	${aegis_LIBRARIES}
	${CPR_LIBRARIES}
	${LIBMONGOCXX_LIBRARIES}
)

target_link_libraries(taiga INTERFACE fifo_map)

target_compile_definitions(taiga PUBLIC
	${LIBMONGOCXX_DEFINITIONS}
)

target_compile_features(taiga PRIVATE cxx_std_17 ${Aegis_CFLAGS_OTHER})
if(MSVC)
	target_compile_options(taiga PRIVATE $<$<CONFIG:RELEASE>:/Ox> /W4)
else()
	target_compile_options(taiga PRIVATE $<$<CONFIG:RELEASE>:-O3> -Wall -Wextra -pedantic)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
	set_target_properties(taiga PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
