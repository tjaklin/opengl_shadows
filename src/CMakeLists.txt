add_library(Interactive Window.cpp Camera.cpp Shader.cpp VertexAttributeParser.cpp Model.cpp Scene.cpp)

target_include_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLEW-CMAKE/include)
target_include_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLFW/include)
target_include_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLM/glm)
target_include_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/STB_IMAGE)
target_include_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/inc)

target_link_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/Vendor)
target_link_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLEW-CMAKE)
target_link_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLFW)
target_link_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLM/glm)
target_link_directories(Interactive PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/STB_IMAGE)

find_package(OpenGL REQUIRED)

target_link_libraries(Interactive PUBLIC OpenGL::GL)
target_link_libraries(Interactive PUBLIC libglew_static)
target_link_libraries(Interactive PUBLIC glfw)
target_link_libraries(Interactive PUBLIC glm)
target_link_libraries(Interactive PUBLIC stb_image)

add_library(ShadowTechniques DirectionalShadowMap.cpp OmnidirectionalShadowMap.cpp VarianceShadowMap.cpp ShadowVolume.cpp DirectionalShadowMapScene.cpp OmnidirectionalShadowMapScene.cpp VarianceShadowMapScene.cpp ShadowVolumeScene.cpp DefaultScene.cpp)

target_include_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLEW-CMAKE/include)
target_include_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLFW/include)
target_include_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLM/glm)
target_include_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/STB_IMAGE)
target_include_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/inc)

target_link_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/Vendor)
target_link_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLEW-CMAKE)
target_link_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLFW)
target_link_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/GLM/glm)
target_link_directories(ShadowTechniques PUBLIC ${PROJECT_SOURCE_DIR}/Vendor/STB_IMAGE)

target_link_libraries(ShadowTechniques PUBLIC OpenGL::GL)
target_link_libraries(ShadowTechniques PUBLIC libglew_static)
target_link_libraries(ShadowTechniques PUBLIC glfw)
target_link_libraries(ShadowTechniques PUBLIC glm)
target_link_libraries(ShadowTechniques PUBLIC stb_image)
target_link_libraries(ShadowTechniques PUBLIC Interactive)