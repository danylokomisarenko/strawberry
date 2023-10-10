include_directories(${SB_CORE_INCLUDE_DIR})
file(GLOB PKG_SOURCES CONFIGURE_DEPENDS "*.h" "*.c" "**/*.h" "**/*.c")
add_executable(sbpkg ${PKG_SOURCES})
target_link_libraries(sbpkg sbcore)
