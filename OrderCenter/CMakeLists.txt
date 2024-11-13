cmake_minimum_required(VERSION 3.10)

project(OrderCenter)

add_executable(client.o
    main.cpp
    order_center.cpp
    cashier.cpp
    simon.cpp
)

target_include_directories(client.o PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(client.o PRIVATE -pthread)
