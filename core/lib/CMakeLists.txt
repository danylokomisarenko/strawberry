macro(update_submodules)
	# Update Submodules
	find_package(Git QUIET)
	if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
		message(STATUS "Updating submodules")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endmacro()

# Add libraries
update_submodules()

set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)

if(WIN32)
	set(ZLIB_LIBRARY "${CMAKE_BINARY_DIR}/core/lib/zlib/zlib.lib")
else()
	set(ZLIB_LIBRARY "${CMAKE_BINARY_DIR}/core/lib/zlib/libz.so")
endif()
set(ZLIB_INCLUDE_DIR "${ZLIB_DIR};${CMAKE_BINARY_DIR}/core/lib/zlib/")

# Add zlib
set(ZLIB_DIR "${CMAKE_CURRENT_LIST_DIR}/zlib")
add_subdirectory(${ZLIB_DIR})
target_include_directories(zlib PUBLIC "${ZLIB_INCLUDE_DIR}")
message(STATUS "Using bundled 'zlib' in '${ZLIB_DIR}'")

# Add curl
set(CURL_DIR "${CMAKE_CURRENT_LIST_DIR}/curl")
set(BUILD_CURL_EXE OFF)
set(CURL_USE_SCHANNEL ON)
set(SSL_ENABLED ON)
set(HTTP_ONLY ON)
add_subdirectory(${CURL_DIR})
message(STATUS "Using bundled 'curl' in '${CURL_DIR}'")

# Add git
set(GIT_DIR "${CMAKE_CURRENT_LIST_DIR}/git")
set(BUILD_TESTS OFF)
set(BUILD_CLI OFF)
add_subdirectory(${GIT_DIR})
target_include_directories(libgit2package PUBLIC "${GIT_DIR}/include")
message(STATUS "Using bundled 'git' in '${GIT_DIR}'")

# Add parson
set(PARSON_DIR "${CMAKE_CURRENT_LIST_DIR}/parson")
set(PARSON_SOURCE "${PARSON_DIR}/parson.h" "${PARSON_DIR}/parson.c")
add_library(parson STATIC ${PARSON_SOURCE})
target_include_directories(parson PUBLIC "${PARSON_DIR}")
message(STATUS "Using bundled 'parson' in '${PARSON_DIR}'")

# Add lua
set(LUA_DIR "${CMAKE_CURRENT_LIST_DIR}/lua")
file(GLOB LUA_SOURCE CONFIGURE_DEPENDS "${LUA_DIR}/*.h" "${LUA_DIR}/*.c" "${LUA_DIR}/**/*.h" "${LUA_DIR}/**/*.c")
add_library(lua STATIC ${LUA_SOURCE})
target_include_directories(lua PUBLIC "${LUA_DIR}")
message(STATUS "Using bundled 'lua' in '${LUA_DIR}'")
