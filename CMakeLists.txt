cmake_minimum_required(VERSION 3.5)
project(Chip8Emu)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Chip8Emu_SOURCE_DIR}/cmake")
# Set an output directory for our binaries
set(BIN_DIR ${Chip8Emu_SOURCE_DIR}/bin)


# Bump up warning levels appropriately for clang, gcc & msvc
# Look up SDL2 and add the include directory to our include path
file(GLOB Chip8Emu_SRC "${Chip8Emu_SOURCE_DIR}/src/*.cpp")
add_executable(Chip8Emu ${Chip8Emu_SRC})
find_package(SDL2 REQUIRED)
set_property(TARGET Chip8Emu PROPERTY CXX_STANDARD 11)
target_include_directories(Chip8Emu PRIVATE ${SDL2_INCLUDE_DIR} include)
target_link_libraries(Chip8Emu ${SDL2_LIBRARY})

# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	target_compile_options(Chip8Emu PUBLIC "-Wall" "-Wextra" "-pedantic" "-Werror")
	target_compile_options(Chip8Emu PUBLIC "$<$<CONFIG:Debug>:-g>")
	target_compile_options(Chip8Emu PUBLIC "$<$<CONFIG:Release>:-O2>")
elseif (MSVC)
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		target_compile_options(Chip8Emu PUBLIC "/W4")
	endif()
endif()

install(TARGETS Chip8Emu RUNTIME DESTINATION ${BIN_DIR})
