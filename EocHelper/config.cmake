if(NOT EocHelper_FOUND)
	set(EocHelper_FOUND True)
	set(EocHelper_INCLUDE_DIRS "")
	set(EocHelper_LIBRARIES EocHelper)
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR} ${CMAKE_BINARY_DIR}/EocHelper)
endif()