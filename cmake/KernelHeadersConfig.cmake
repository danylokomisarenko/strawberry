if (NOT LINUX)
	message(STATUS "you are not on linux, skipping kernel header detection")
	return()
endif()

# get kernel version
execute_process(
	COMMAND uname -r
	OUTPUT_VARIABLE KERNEL_RELEASE
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "detected kernel version '${KERNEL_RELEASE}'")

# get by kernel version
find_path(KERNELHEADERS_DIR
	include/linux/user.h
	PATHS /usr/src/linux-headers-${KERNEL_RELEASE}
)

if(NOT KERNELHEADERS_DIR)
	# default to first directory
	message(WARNING "defaulting to first directory")
	file(GLOB KERNELHEADER_DIRS "usr/src/**")
	foreach(DIRECTORY ${KERNELHEADER_DIRS})
		message(STATUS "testing ${DIRECTORY}")
		if(IS_DIRECTORY ${DIRECTORY})
			set(KERNELHEADERS_DIR ${DIRECTORY})
			break()
		else()
			continue()
		endif()
	endforeach()
endif()

if (KERNELHEADERS_DIR)
	set(KERNELHEADERS_INCLUDE_DIRS
		${KERNELHEADERS_DIR}/include
		${KERNELHEADERS_DIR}/arch/x86/include
		CACHE PATH "kernel headers include dir"
	)
	set(KERNELHEADERS_FOUND ON)
else()
	set(KERNELHEADERS_FOUND OFF)
endif()

if (KERNELHEADERS_FOUND)
	message(STATUS "found kernel include dirs '${KERNELHEADERS_INCLUDE_DIRS}'")
else()
	message(WARNING "warning: kernel headers could not be found")
endif()

mark_as_advanced(KERNELHEADERS_FOUND)
