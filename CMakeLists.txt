cmake_minimum_required(VERSION 3.8)
project("strawberry" VERSION 0.1 LANGUAGES C)

set(SB_CORE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/core/include")
set(SB_CORE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/core/src")
add_subdirectory("core")
add_subdirectory("pkg")
add_subdirectory("gateway")

# Copy binaries
set(GATEWAY_EXE_NAME)
set(PKG_EXE_NAME)
set(CORE_DLL_NAME)
if(WIN32)
	set(GATEWAY_EXE_NAME "sbgateway.exe")
	set(PKG_EXE_NAME "sbpkg.exe")
	set(CORE_DLL_NAME "sbcore.dll")
else()
	set(GATEWAY_EXE_NAME "sbgateway")
	set(PKG_EXE_NAME "sbpkg")
	set(CORE_DLL_NAME "libsbcore.so")
endif()

message(STATUS "Link '${CMAKE_BINARY_DIR}/compile_commands.json' to '${CMAKE_SOURCE_DIR}/compile_commands.json'")
file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  SYMBOLIC
)

set(RUN_DIR "${CMAKE_SOURCE_DIR}/run")
add_custom_target(copybinaries ALL
    DEPENDS sbgateway sbpkg sbcore
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${RUN_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${RUN_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/gateway/${GATEWAY_EXE_NAME} ${RUN_DIR}/${GATEWAY_EXE_NAME}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/pkg/${PKG_EXE_NAME} ${RUN_DIR}/${PKG_EXE_NAME}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/core/src/${CORE_DLL_NAME} ${RUN_DIR}/${CORE_DLL_NAME}
)
