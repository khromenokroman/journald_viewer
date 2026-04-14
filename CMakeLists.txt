cmake_minimum_required(VERSION 3.25.1)
project(journald-viewer
        VERSION 0.2.0
        LANGUAGES CXX
        HOMEPAGE_URL "https://github.com/khromenokroman/journald_viewer")

set(CMAKE_CXX_STANDARD 20)

configure_file(Version.in Version @ONLY)

find_package(fmt REQUIRED)
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/journald_viewer.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/journald
        ${CMAKE_CURRENT_BINARY_DIR}/tests/journald
        COPYONLY
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/journald.expected
        ${CMAKE_CURRENT_BINARY_DIR}/tests/journald.expected
        COPYONLY
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

enable_testing()
add_executable(journald_viewer_tests
        tests/tests.cpp
)
target_link_libraries(journald_viewer_tests PRIVATE GTest::gtest GTest::gtest_main)
add_test(NAME journald_viewer_tests COMMAND journald_viewer_tests)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "khromenokroman")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "khromenokroman")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Journald viewer")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "khromenokroman")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)