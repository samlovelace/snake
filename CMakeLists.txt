cmake_minimum_required(VERSION 3.8)
project(snake)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(cppzmq REQUIRED)

# ONNX Runtime for the --dqn inference agent. Downloaded via FetchContent
# (no CMake config in the prebuilt archive, so it's wired up manually below).
include(FetchContent)
set(SNAKE_ORT_VERSION "1.20.1" CACHE STRING "ONNX Runtime version")
FetchContent_Declare(onnxruntime
  URL "https://github.com/microsoft/onnxruntime/releases/download/v${SNAKE_ORT_VERSION}/onnxruntime-linux-x64-${SNAKE_ORT_VERSION}.tgz")
FetchContent_MakeAvailable(onnxruntime)

find_library(ORT_LIB onnxruntime PATHS ${onnxruntime_SOURCE_DIR}/lib NO_DEFAULT_PATH REQUIRED)

set(SNAKE_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/Segment.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Snake.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Apple.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/AI.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Scene.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TrainingAgent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/DqnState.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/DqnAgent.cpp
)

add_executable(${PROJECT_NAME} ${SNAKE_SOURCES})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${onnxruntime_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system cppzmq ${ORT_LIB})

# Bake the ORT shared library location into the binary's RPATH so it runs
# without needing LD_LIBRARY_PATH set manually.
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-rpath,${onnxruntime_SOURCE_DIR}/lib")

install(TARGETS ${PROJECT_NAME} DESTINATION ../)