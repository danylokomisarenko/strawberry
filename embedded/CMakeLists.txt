message(STATUS "using 'corrosion' for cmake and rust interop")
include(FetchContent REQUIRED)
FetchContent_Declare(
	Corrosion
	GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
	GIT_TAG v0.4
)
