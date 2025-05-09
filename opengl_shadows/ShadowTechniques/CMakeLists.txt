add_library(ShadowTechniques)

target_sources(ShadowTechniques
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DirectionalShadowMap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/OmnidirectionalShadowMap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VarianceShadowMap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ShadowVolume.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DirectionalShadowMapScene.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/OmnidirectionalShadowMapScene.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VarianceShadowMapScene.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ShadowVolumeScene.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultScene.hpp

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/DirectionalShadowMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/OmnidirectionalShadowMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/VarianceShadowMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/ShadowVolume.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/DirectionalShadowMapScene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/OmnidirectionalShadowMapScene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/VarianceShadowMapScene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/ShadowVolumeScene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/DefaultScene.cpp
)

target_include_directories(ShadowTechniques
    INTERFACE
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>

    PUBLIC
    ${PROJECT_SOURCE_DIR}/Vendor/GLEW-CMAKE
    ${PROJECT_SOURCE_DIR}/Vendor/GLFW
    ${PROJECT_SOURCE_DIR}/Vendor/GLM/glm
    ${PROJECT_SOURCE_DIR}/Vendor/STB_IMAGE
)

find_package(OpenGL REQUIRED)

## TODO: Ovo pregledat kak sredit. Pitati se ista pitanja koja su raspisana
#   kod definicije 'Interactive' library-ja.
target_link_directories(ShadowTechniques
    PUBLIC
    ${PROJECT_SOURCE_DIR}/Vendor/GLEW-CMAKE
    ${PROJECT_SOURCE_DIR}/Vendor/GLFW
    ${PROJECT_SOURCE_DIR}/Vendor/GLM/glm
    ${PROJECT_SOURCE_DIR}/Vendor/STB_IMAGE
)

target_link_libraries(ShadowTechniques
    PUBLIC
    OpenGL::GL
    libglew_static
    glfw
    glm
    stb_image
    Interactive
)
