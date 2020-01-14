cmake_minimum_required(VERSION 3.12)
project(csc305_labs LANGUAGES CXX C)

set(LABS_ROOT "${PROJECT_SOURCE_DIR}/labs")

include(FetchContent)
FetchContent_Declare(
    atlas
    GIT_REPOSITORY https://github.com/marovira/atlas.git
    )
FetchContent_Populate(atlas)
add_subdirectory(${atlas_SOURCE_DIR} ${atlas_BINARY_DIR})

add_subdirectory(${LABS_ROOT})
