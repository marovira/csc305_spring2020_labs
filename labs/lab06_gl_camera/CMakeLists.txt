get_filename_component(LAB_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

set(LAB_ROOT "${LABS_ROOT}/${LAB_NAME}")

set(SOURCE_LIST 
    "${LAB_ROOT}/main.cpp"
    )

set(INCLUDE_LIST
    "${LAB_ROOT}/lab.hpp"
    )

set(PATH_INCLUDE "${LAB_ROOT}/paths.hpp")
configure_file("${LAB_ROOT}/paths.hpp.in" ${PATH_INCLUDE})
list(APPEND INCLUDE_LIST ${PATH_INCLUDE})

source_group("source" FILES ${SOURCE_LIST})
source_group("include" FILES ${INCLUDE_LIST})

add_executable(${LAB_NAME} ${SOURCE_LIST} ${INCLUDE_LIST})
target_include_directories(${LAB_NAME} PUBLIC ${LAB_ROOT})
target_link_libraries(${LAB_NAME} PUBLIC atlas::atlas)
set_target_properties(${LAB_NAME} PROPERTIES FOLDER "labs")
