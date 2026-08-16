cmake_minimum_required(VERSION 3.8)
project(snake) 

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(SNAKE_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/src/Segment.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Snake.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Apple.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/AI.cpp
)

add_executable(${PROJECT_NAME} ${SNAKE_SOURCES})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)