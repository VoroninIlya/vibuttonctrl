cmake_minimum_required(VERSION 3.22)

project(vibuttonctrl)

# Debug message
message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

# Register core library
add_library(vibuttonctrl INTERFACE)
target_sources(vibuttonctrl PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/vibuttonctrl.c)
target_include_directories(vibuttonctrl INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/inc)

# Debug message
message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")