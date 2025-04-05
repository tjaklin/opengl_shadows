#include "../inc/OmnidirectionalShadowMapScene.hpp"

OmnidirectionalShadowMapScene::OmnidirectionalShadowMapScene(Window* window)
    : Scene(window)
{}

void OmnidirectionalShadowMapScene::Run() const
{
    Camera eye;
    glm::vec3 eyePosition = glm::vec3(5.0f, 10.0f, 5.0f);
    eye.SetViewMatrix(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0,1,0));
    eye.SetPerspectiveProjectionMatrix(45.0f, 4.0f/3.0f, 1.0f, 100.0f);

    _window->SetCamera(&eye);

    Camera pointLight;
    glm::vec3 pointLightPosition = glm::vec3(10.0f, 15.0f, -10.0f);
    pointLight.SetViewMatrix(pointLightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0,1,0));
    //pointLight.SetOrthogonalProjectionMatrix(); -> Set the Projection matrix !

    const char* cube_position_filepath = "vertices/cube_position.txt";
    VertexAttribute position = VertexAttributeParser::ProcessFile(cube_position_filepath);
    if (position.data.empty())
    {
        // An error occurred in getting vertex data.
        printf("[OmnidirectionalShadowMapScene] VertexAttribute 'position' error.\n");
        return;
    }
    
    const char* cube_normal_filepath = "vertices/cube_normal.txt";
    VertexAttribute normal = VertexAttributeParser::ProcessFile(cube_normal_filepath);
    if (normal.data.empty())
    {
        // An error occurred in getting vertex data.
        printf("[OmnidirectionalShadowMapScene] VertexAttribute 'normal' error.\n");
        return;
    }

    Model cube;
    cube.PushVertexAttribute(position, 0);
    cube.PushVertexAttribute(normal, 1);
    // Don't perform any Model matrix manipulation for 'cube' yet.

    Model podloga;
    podloga.PushVertexAttribute(position, 0);
    podloga.PushVertexAttribute(normal, 1);
    podloga.SetScale(glm::vec3(15.0f, 1.0f, 15.0f));
    podloga.SetTranslation(glm::vec3(0.0f, -5.0f, 0.0f));

    const char* vertexDefaultFilepath = "shaders/default.vs";
    const char* fragmentDefaultFilepath = "shaders/default.fs";
    Shader defaultShader(vertexDefaultFilepath, fragmentDefaultFilepath);

    defaultShader.Bind();
    auto activeShader = defaultShader.Get();

    // TODO: Add the actual 'OmnidirectionalShadowMap' object and set
    //  everything else up.
    //OmnidirectionalShadowMap shadowMap(1024, 1024, 1024, 1024);
    //shadowMap.LoadShaders();
    //shadowMap.PrepareFBOandTexture();

    while(!_window->ShouldClose())
    {
        _window->Clear();
        _window->Update();

        // Set 'global' uniform data.
        glUniformMatrix4fv(glGetUniformLocation(activeShader, "view"),
            1, GL_FALSE, &eye.GetViewMatrix()[0][0]);

        glUniformMatrix4fv(glGetUniformLocation(activeShader, "projection"),
            1, GL_FALSE, &eye.GetProjectionMatrix()[0][0]);

        // Set 'cube' specific data.
        glUniformMatrix4fv(glGetUniformLocation(activeShader, "model"),
            1, GL_FALSE, &cube.GetModelMatrix()[0][0]);

        cube.Draw();

        // Set 'podloga' specific data.
        glUniformMatrix4fv(glGetUniformLocation(activeShader, "model"),
            1, GL_FALSE, &podloga.GetModelMatrix()[0][0]);

        podloga.Draw();

        _window->Draw();
    }
}