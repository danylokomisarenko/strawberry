configure_file("${SB_CORE_SOURCE_DIR}/sbconf.h.in" "${SB_CORE_INCLUDE_DIR}/strawberry/sbconf.h")
add_subdirectory("lib")
add_subdirectory("src")
