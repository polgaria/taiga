set(HEADER_LIST
	"${taiga_SOURCE_DIR}/include/taiga/Command.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/Commands.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/Config.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/util/Util.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/util/MathUtil.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/util/StringUtil.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/util/CommandUtil.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/Client.hpp"
) 

add_library(taiga
	util/Util.cpp
	util/StringUtil.cpp
	util/CommandUtil.cpp
	Command.cpp
	Commands.cpp
	Client.cpp
	${HEADER_LIST}
)

target_include_directories(taiga PUBLIC
	../include
	${fifo_map_INCLUDE_DIRS}
	${CPR_INCLUDE_DIRS}
	${LIBMONGOCXX_INCLUDE_DIRS}
	${Aegis_INCLUDE_DIRS}
)

target_link_libraries(taiga PUBLIC
	date_interface
	tz
	fifo_map
	${CPR_LIBRARIES}
	${LIBMONGOCXX_LIBRARIES}
	${Aegis_LIBRARIES}
)

target_compile_definitions(taiga PUBLIC
	${LIBMONGOCXX_DEFINITIONS}
)

target_compile_features(taiga PUBLIC cxx_std_17 ${Aegis_CFLAGS_OTHER}) 

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
