cmake_minimum_required(VERSION 4.2)
project(journal_pretty)
find_package(fmt REQUIRED)
set(CMAKE_CXX_STANDARD 20)

add_executable(journal_pretty main.cpp)

target_link_libraries(journal_pretty PRIVATE fmt::fmt)
