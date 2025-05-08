# Set the LittleFS root directory
set(LFS_DIR "${CMAKE_SOURCE_DIR}/third-party/LittleFS")

# Build LittleFS library
add_library(lfs INTERFACE)

# Set sources used for LittleFS components
file(GLOB_RECURSE SOURCES
    ${LFS_DIR}/*.c
)

target_sources(lfs INTERFACE
    ${SOURCES}
)

# Include root's header
target_include_directories(lfs INTERFACE
    ${LFS_DIR}
)
