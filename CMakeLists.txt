cmake_minimum_required(VERSION 3.30.1)
project("udp-packet-recovery" LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE MAIN_SRC "${CMAKE_SOURCE_DIR}/src/main/*.c")

set(MAIN_INC "${CMAKE_SOURCE_DIR}/include/main/")

add_executable("${PROJECT_NAME}" "${MAIN_SRC}")
target_include_directories("${PROJECT_NAME}" PRIVATE "${MAIN_INC}")
set_property(TARGET "${PROJECT_NAME}" PROPERTY C_STANDARD 23)

if("${CMAKE_BUILD_TYPE}" EQUAL "Debug")
  target_compile_definitions("${PROJECT_NAME}" PRIVATE DEBUG)
endif()

