# Set the LVGL root directory
set(LVGL_ROOT_DIR   "${CMAKE_SOURCE_DIR}/third-party/LVGL")
set(LVGL_SRC_DIR    "${LVGL_ROOT_DIR}/lvgl")
set(LVGL_PORT_DIR   "${LVGL_ROOT_DIR}/porting")

# Build LVGL library
add_library(lvgl INTERFACE)

# Set sources used for LVGL components
file(GLOB_RECURSE SOURCES
    ${LVGL_SRC_DIR}/src/*.c 
    ${LVGL_SRC_DIR}/src/*.S 
    ${LVGL_PORT_DIR}/*.c
)

target_sources(lvgl INTERFACE
    ${SOURCES}
)

# Include root and optional parent path of LV_CONF_PATH
target_include_directories(lvgl INTERFACE
    ${LVGL_ROOT_DIR}
)
