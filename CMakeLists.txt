cmake_minimum_required(VERSION 3.0.0)
project(terminal_cam VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)

include_directories((${OpenCV_INCLUDE_DIRS}))
include_directories((/usr/include/ncurses))

add_executable(terminal_cam main.cpp)

target_link_libraries(terminal_cam ${OpenCV_LIBS} ncurses)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
