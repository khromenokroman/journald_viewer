cmake_minimum_required(VERSION 4.2)
project(journal_pretty)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt REQUIRED)

add_executable(journal_pretty
        src/main.cpp
        src/journald_viewer.cpp
)

target_link_libraries(journal_pretty PRIVATE fmt::fmt)
