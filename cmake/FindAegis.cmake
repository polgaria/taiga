find_path(Aegis_INCLUDE_DIR NAMES "aegis.hpp" PATHS "${aisaka_SOURCE_DIR}/lib/aegis/include")
if(Aegis_INCLUDE_DIR STREQUAL "Aegis_INCLUDE_DIR-NOTFOUND")
	message(FATAL_ERROR "Could not find Aegis.")
	set(Aegis_FOUND FALSE)
endif()

set(Aegis_FOUND TRUE)

if(Aegis_FOUND)
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(Aegis
		REQUIRED_VARS Aegis_INCLUDE_DIR
		FOUND_VAR Aegis_FOUND
		FAIL_MESSAGE "Could not find Aegis.")

	if(NOT TARGET Aegis)
		add_library(Aegis INTERFACE IMPORTED)
		set_target_properties(Aegis PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES ${Aegis_INCLUDE_DIR})
	endif()

    mark_as_advanced(Aegis_INCLUDE_DIR)
endif()
