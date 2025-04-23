cmake_minimum_required(VERSION 3.20)
project(Sjene VERSION 0.2.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/)

add_subdirectory(${PROJECT_SOURCE_DIR}/Vendor)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} Application.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc)

target_link_libraries(${PROJECT_NAME} PUBLIC Interactive) # Defined in src/
target_link_libraries(${PROJECT_NAME} PUBLIC ShadowTechniques) # Defined in src/