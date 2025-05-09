add_library(Interactive)

target_sources(Interactive
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Window.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Shader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VertexAttributeParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Model.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene.hpp

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/VertexAttributeParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Scene.cpp
)

target_include_directories(Interactive
    INTERFACE
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>

    PRIVATE
    ${PROJECT_SOURCE_DIR}/Vendor/GLEW-CMAKE
    ${PROJECT_SOURCE_DIR}/Vendor/GLFW
    ${PROJECT_SOURCE_DIR}/Vendor/GLM/glm
    ${PROJECT_SOURCE_DIR}/Vendor/STB_IMAGE
)

find_package(OpenGL REQUIRED)

## TODO: Ovo pregledat kak sredit.
#   Dal ima potrebe ovo koristiti, bi li mi se pronasle sve
#   knjiznice i bez ove naredbe ?
target_link_directories(Interactive
    PUBLIC
    ${PROJECT_SOURCE_DIR}/Vendor
    ${PROJECT_SOURCE_DIR}/Vendor/GLEW-CMAKE
    ${PROJECT_SOURCE_DIR}/Vendor/GLFW
    ${PROJECT_SOURCE_DIR}/Vendor/GLM/glm
    ${PROJECT_SOURCE_DIR}/Vendor/STB_IMAGE
)

target_link_libraries(Interactive
    PUBLIC
    OpenGL::GL
    libglew_static
    glfw
    glm
    stb_image
)
