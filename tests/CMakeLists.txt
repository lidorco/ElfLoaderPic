cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)

message("Compile tests...")

add_executable(pic_runner pic_runner.c)

set(CMAKE_C_FLAGS "-m32 -g")

set_target_properties(pic_runner PROPERTIES PREFIX "")
set_target_properties(pic_runner PROPERTIES OUTPUT_NAME "pic_runner")
set_target_properties(pic_runner PROPERTIES SUFFIX ".elf")
