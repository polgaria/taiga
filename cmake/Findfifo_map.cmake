find_path(fifo_map_INCLUDE_DIR NAMES "fifo_map.hpp" PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/fifo_map/src")
if(fifo_map_INCLUDE_DIR STREQUAL "fifo_map_INCLUDE_DIR-NOTFOUND")
	message(FATAL_ERROR "Could not find nlohmann/fifo_map.")
	set(fifo_map_FOUND FALSE)
endif()

set(fifo_map_FOUND TRUE)

if(fifo_map_FOUND)
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(fifo_map
		REQUIRED_VARS fifo_map_INCLUDE_DIR
		FOUND_VAR fifo_map_FOUND
		FAIL_MESSAGE "Could not find nlohmann/fifo_map.")

	if(NOT TARGET fifo_map)
		add_library(fifo_map INTERFACE IMPORTED)
		set_target_properties(fifo_map PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${fifo_map_INCLUDE_DIR})
	endif()

    mark_as_advanced(fifo_map_INCLUDE_DIR)
endif()
