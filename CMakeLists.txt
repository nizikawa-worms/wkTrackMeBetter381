cmake_minimum_required(VERSION 3.23)
project(wkTrackMeBetter381 VERSION "1.0.0.0" DESCRIPTION "Tune the in-game camera tracking instead of Ctrl+Home.")

set(CMAKE_CXX_STANDARD 20)
configure_file(resource.rc.in resource.rc)
add_library(${PROJECT_NAME} SHARED dllmain.cpp ${CMAKE_CURRENT_BINARY_DIR}/resource.rc)
target_link_libraries(${PROJECT_NAME} version)
set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

