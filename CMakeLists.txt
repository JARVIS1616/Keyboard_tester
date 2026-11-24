cmake_minimum_required(VERSION 3.20)
project(keyboard_tester C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSES REQUIRED ncurses)

add_executable(keyboard_tester main.c)

target_include_directories(keyboard_tester PRIVATE ${NCURSES_INCLUDE_DIRS})
target_link_directories(keyboard_tester PRIVATE ${NCURSES_LIBRARY_DIRS})
target_link_libraries(keyboard_tester PRIVATE ${NCURSES_LIBRARIES})