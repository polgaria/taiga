set(HEADER_LIST
	"${taiga_SOURCE_DIR}/include/taiga/command/Command.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/command/categories/Category.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/command/categories/General.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/command/categories/Reddit.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/command/categories/Conversion.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/command/categories/Timezone.hpp"
	"${taiga_SOURCE_DIR}/include/taiga/command/categories/Miscellaneous.hpp"
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
	command/Command.cpp
	command/categories/Category.cpp
	command/categories/General.cpp
	command/categories/Reddit.cpp
	command/categories/Conversion.cpp
	command/categories/Timezone.cpp
	command/categories/Miscellaneous.cpp
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
