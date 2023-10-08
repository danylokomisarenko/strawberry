include_directories("${SB_CORE_INCLUDE_DIR}")
file(GLOB GATEWAY_SOURCE CONFIGURE_DEPENDS "*.h" "*.c" "**\*.h" "**\*.c")
add_executable(sbgateway ${GATEWAY_SOURCE})
