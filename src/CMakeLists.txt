set(HEADER_LIST
	"${taigabot_SOURCE_DIR}/include/taigabot/Command.hpp"
	"${taigabot_SOURCE_DIR}/include/taigabot/Commands.hpp"
	"${taigabot_SOURCE_DIR}/include/taigabot/Config.hpp"
	"${taigabot_SOURCE_DIR}/include/taigabot/util/Util.hpp"
	"${taigabot_SOURCE_DIR}/include/taigabot/util/MathUtil.hpp"
	"${taigabot_SOURCE_DIR}/include/taigabot/util/StringUtil.hpp"
	"${taigabot_SOURCE_DIR}/include/taigabot/Client.hpp"
) 

add_library(taigabot
	util/Util.cpp
	util/StringUtil.cpp
	Command.cpp
	Commands.cpp
	Client.cpp
	${HEADER_LIST}
)

target_include_directories(taigabot PUBLIC
	../include
	${CPR_INCLUDE_DIRS}
	${LIBMONGOCXX_INCLUDE_DIRS}
	${Aegis_INCLUDE_DIRS}
)

target_link_libraries(taigabot PUBLIC
	rapidjson
	date_interface
	tz
	${CPR_LIBRARIES}
	${LIBMONGOCXX_LIBRARIES}
	${Aegis_LIBRARIES}
)

target_compile_definitions(taigabot PUBLIC
	${LIBMONGOCXX_DEFINITIONS}
)

target_compile_features(taigabot PUBLIC cxx_std_17 ${Aegis_CFLAGS_OTHER}) 

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
