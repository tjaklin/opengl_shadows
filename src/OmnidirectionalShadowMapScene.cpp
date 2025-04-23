#include "../inc/OmnidirectionalShadowMapScene.hpp"

OmnidirectionalShadowMapScene::OmnidirectionalShadowMapScene(Window* window)
    : Scene(window)
{}

void OmnidirectionalShadowMapScene::Run() const
{
    Camera eye;
    glm::vec3 eyePosition = glm::vec3(0.0f, 5.0f, 20.0f);
    eye.SetViewMatrix(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0,1,0));
    eye.SetPerspectiveProjectionMatrix(45.0f, 4.0f/3.0f, 1.0f, 100.0f);

    Camera light;
    glm::vec3 lightPosition = glm::vec3(10.0f, 15.0f, 10.0f);
    light.SetPerspectiveProjectionMatrix(glm::radians(90.0f), 1.0f, 1.0f, 100.0f);
    light.SetViewAndProjectionMatrix(lightPosition);

    _window->SetCamera(&eye);

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

    Model kocka1;
    kocka1.PushVertexAttribute(position, 0);
    kocka1.PushVertexAttribute(normal, 1);
    auto kocka1Model = kocka1.GetModelMatrix();
    // Don't perform any Model matrix manipulation for 'kocka1' yet.

    Model kocka2;
    kocka2.PushVertexAttribute(position, 0);
    kocka2.PushVertexAttribute(normal, 1);
    kocka2.SetTranslation(glm::vec3(-7.0f, -2.0f, 5.0f));
    auto kocka2Model = kocka2.GetModelMatrix();

    Model podloga;
    podloga.PushVertexAttribute(position, 0);
    podloga.PushVertexAttribute(normal, 1);
    podloga.SetScale(glm::vec3(15.0f, 1.0f, 15.0f));
    podloga.SetTranslation(glm::vec3(0.0f, -5.0f, 0.0f));
    auto podlogaModel = podloga.GetModelMatrix();

    // Shaders; both depth and light.
    const char* vertexDepthFilepath = "shaders/shadow_map/omnidirectional/depth.vs";
    const char* geometryDepthFilepath = "shaders/shadow_map/omnidirectional/depth.gs";
    const char* fragmentDepthFilepath = "shaders/shadow_map/omnidirectional/depth.fs";
    Shader depthShader(vertexDepthFilepath, geometryDepthFilepath, fragmentDepthFilepath);

    const char* vertexLightFilepath = "shaders/shadow_map/omnidirectional/light.vs";
    const char* fragmentLightFilepath = "shaders/shadow_map/omnidirectional/light.fs";
    Shader lightShader(vertexLightFilepath, fragmentLightFilepath);

    // TODO: Add support for 'OmnidirectionalShadowMapp::DebugPassSetup()' and
    //  add the debug shaders here.

    OmnidirectionalShadowMap shadowMap(1024, 1024, 1024, 1024);
    shadowMap.LoadShaders(&depthShader, &lightShader);
    shadowMap.PrepareFBOandTexture();

    // Prepare for the main Scene loop.
    while(!_window->ShouldClose())
    {
        _window->Clear();
        _window->Update();

        // Prepare for the First rendering pass, which will
        //  fill the contents of a 'depth map'.
        shadowMap.FirstPassSetup();
        shadowMap.SetShadowMatrices(light.GetViewAndProjectionMatrix());
        shadowMap.SetLightPosition_Depth(lightPosition);
        shadowMap.SetFarPlane_Depth(100.0f);

        shadowMap.SetModelMatrix_Depth(kocka1Model);
        kocka1.Draw();

        shadowMap.SetModelMatrix_Depth(kocka2Model);
        kocka2.Draw();

        shadowMap.SetModelMatrix_Depth(podlogaModel);
        podloga.Draw();

        // Second rendering pass.
        shadowMap.SecondPassSetup();
        shadowMap.SetViewMatrix(eye.GetViewMatrix());
        shadowMap.SetProjectionMatrix(eye.GetProjectionMatrix());
        shadowMap.SetDepthTexture();
        shadowMap.SetLightPosition_Light(lightPosition);
        shadowMap.SetFarPlane_Light(100.0f);

        shadowMap.SetModelMatrix_Light(kocka1Model);
        kocka1.Draw();

        shadowMap.SetModelMatrix_Light(kocka2Model);
        kocka2.Draw();

        shadowMap.SetModelMatrix_Light(podlogaModel);
        podloga.Draw();

		// Draw the parent object's (Scene.cpp) 'PostDrawHook()'. It draws
		//	a GUI on top of our scene.
		PostDrawHook();

        _window->Draw();
    }
}