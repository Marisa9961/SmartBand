cmake_minimum_required(VERSION 3.28)

# ------------------------------ Project Config -------------------------------- #
set(CMAKE_PROJECT_NAME "target")

include(cmake/toolchain.cmake)
include(cmake/stm32cubemx.cmake)

project(${CMAKE_PROJECT_NAME} LANGUAGES C CXX ASM)
add_executable(${CMAKE_PROJECT_NAME}.elf)

add_subdirectory(third-party/lvgl)
add_subdirectory(third-party/LittleFS)

# ------------------------------ Directory Path ------------------------------ #
set(APP_PATH    "app")
set(THIRD_PATH  "third-party")

# ------------------------------- Source Files ------------------------------- #
file(GLOB_RECURSE sources_file 
    "${APP_PATH}/*.c" 
    "${APP_PATH}/*.cpp" 
)

target_include_directories(${CMAKE_PROJECT_NAME}.elf PRIVATE
    ${APP_PATH}
)

target_sources(${CMAKE_PROJECT_NAME}.elf PRIVATE
    ${sources_file}
)

# ------------------------------- Third Party --------------------------------- #
target_link_libraries(${CMAKE_PROJECT_NAME}.elf PRIVATE
    stm32cubemx
    lvgl
    lfs
)

# ------------------------------- After Build --------------------------------- #
set(hex_file ${CMAKE_PROJECT_NAME}.hex)
set(bin_file ${CMAKE_PROJECT_NAME}.bin)
set(asm_file ${CMAKE_PROJECT_NAME}.s)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}.elf> ${hex_file}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${CMAKE_PROJECT_NAME}.elf> ${bin_file}
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:${CMAKE_PROJECT_NAME}.elf> >${asm_file}
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}.elf>
)
