cmake_minimum_required(VERSION 3.15)

project(InventoryManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(InventoryManagementSystem
    src/main.cpp
    src/Item.cpp
    src/InventoryManager.cpp
)

target_include_directories(InventoryManagementSystem PRIVATE src)
