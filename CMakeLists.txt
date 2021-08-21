cmake_minimum_required(VERSION 3.10)
project(ElfLoaderPic C)

set(CMAKE_C_STANDARD 11)

message("Building 32 bit PIC...")

add_executable(shellcode src/main.c)

get_filename_component(CMAKE_CURRENT_SOURCE_ABSOLUTE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

# -fno-pie - remove the default __x86_get_pc_thunk_bx - loads the position of the code into the %ebx register.
# -fomit-frame-pointer - remove the "push ebp; mov esp ebp;"  at beginning of any function.
# -T linker_script - it order the linker which function to put at the beginning of the pic_code segment.
set_target_properties(shellcode PROPERTIES COMPILE_FLAGS "-m32 -fno-pie -fomit-frame-pointer"
                                           LINK_FLAGS "-m32 -T ${CMAKE_CURRENT_SOURCE_ABSOLUTE_DIR}/build/linker_script")

set_target_properties(shellcode PROPERTIES PREFIX "")
set_target_properties(shellcode PROPERTIES OUTPUT_NAME "shellcode")
set_target_properties(shellcode PROPERTIES SUFFIX ".elf")


add_custom_command(COMMAND objcopy --dump-section .pic_code=shellcode.pic ${CMAKE_CURRENT_SOURCE_DIR}/shellcode.elf
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   OUTPUT  shellcode.pic
                   COMMENT "Extracting shellcode.pic from shellcode.elf...")

add_custom_target(shellcodePic ALL
                  DEPENDS shellcode.elf
                  shellcode.pic)

add_dependencies(shellcodePic shellcode)


add_subdirectory(tests)
