include_directories("${SB_CORE_INCLUDE_DIR}")
include_directories("${SB_CORE_SOURCE_DIR}")
file(GLOB CORE_SOURCE CONFIGURE_DEPENDS "*.h" "*.c" "**\*.h" "**\*.c")
