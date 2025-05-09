cmake_minimum_required(VERSION 3.20)
project(Sjene VERSION 0.2.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_subdirectory(Vendor)
add_subdirectory(opengl_shadows)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE Application.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Interactive)
target_link_libraries(${PROJECT_NAME} PRIVATE ShadowTechniques)
