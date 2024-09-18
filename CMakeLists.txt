cmake_minimum_required(VERSION 3.8)
project(snake) 

include(${CMAKE_CURRENT_LIST_DIR}/src/CMakeLists.txt)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(${PROJECT_NAME} ${SNAKE_SOURCES})

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)