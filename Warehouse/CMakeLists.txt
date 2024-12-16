cmake_minimum_required(VERSION 3.10)

project(Warehouse)

add_executable(server.o
    warehouse_main.cpp
    dan.cpp
    server.cpp
)

target_include_directories(server.o PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(server.o PRIVATE -pthread)
