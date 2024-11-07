cmake_minimum_required(VERSION 3.8)
project(snake) 

# Define warning flags based on the compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Enable all warnings for gcc and clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Enable warnings for MSVC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/src/CMakeLists.txt)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(${PROJECT_NAME} ${SNAKE_SOURCES})

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)