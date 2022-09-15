cmake_minimum_required(VERSION 3.23)
project(wkTrackMeBetter381 VERSION "1.0.0.0" DESCRIPTION "Tune the in-game camera tracking instead of Ctrl+Home.")

set(CMAKE_CXX_STANDARD 20)
configure_file(resource.rc.in resource.rc)
add_library(${PROJECT_NAME} SHARED dllmain.cpp ${CMAKE_CURRENT_BINARY_DIR}/resource.rc)
target_link_libraries(${PROJECT_NAME} version)
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE /Zi)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_PDB_NAME ${PROJECT_NAME} COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR})
endif()
