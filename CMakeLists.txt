cmake_minimum_required(VERSION 3.30.1)
project("udp-packet-recovery" LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE MAIN_SRC "${CMAKE_SOURCE_DIR}/src/main/*.c")
file(GLOB_RECURSE LIB_SRC "${CMAKE_SOURCE_DIR}/src/lib/*.c")

set(MAIN_INC "${CMAKE_SOURCE_DIR}/include/main/")
set(LIB_INC "${CMAKE_SOURCE_DIR}/include/lib/")

add_executable("${PROJECT_NAME}" "${MAIN_SRC}" "${LIB_SRC}")

target_include_directories("${PROJECT_NAME}" PRIVATE "${MAIN_INC}" "${LIB_INC}")

target_compile_options("${PROJECT_NAME}" PRIVATE -g3)
target_compile_definitions("${PROJECT_NAME}" PRIVATE DEBUG)
