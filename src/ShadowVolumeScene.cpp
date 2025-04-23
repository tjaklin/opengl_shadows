#include "ShadowVolumeScene.hpp"

ShadowVolumeScene::ShadowVolumeScene(Window* window)
    : Scene(window)
{}

void ShadowVolumeScene::Run() const
{
    Camera eye;
    glm::vec3 eyePosition = glm::vec3(0.0f, 5.0f, 20.0f);
    eye.SetViewMatrix(eyePosition, glm::vec3(0.0f), glm::vec3(0,1,0));
    eye.SetPerspectiveProjectionMatrix(45.0f, 4.0f/3.0f, 1.0f, 100.0f);

    Camera light;
    glm::vec3 lightPosition = glm::vec3(10.0f, 15.0f, 10.0f);
    light.SetViewAndProjectionMatrix(lightPosition);
    light.SetPerspectiveProjectionMatrix(glm::radians(90.0f), 1.0f, 1.0f, 100.0f);
    
    _window->SetCamera(&eye);

    // Prepare 3D shape data.
	const char* cube_position_filepath = "vertices/cube_position.txt";
	VertexAttribute position = VertexAttributeParser::ProcessFile(cube_position_filepath);
	if (position.data.empty())
    {
        printf("VertexAttribute is empty. Exiting.\n");
        return;
    }
	
	const char* cube_normal_filepath = "vertices/cube_normal.txt";
	VertexAttribute normal = VertexAttributeParser::ProcessFile(cube_normal_filepath);
	if (normal.data.empty())
    {
        printf("VertexAttribute is empty. Exiting.\n");
        return;
    }

    // Prepare 2D shape data (For rectangle drawing).
    const char* rectangle_position_filepath = "vertices/rectangle_position.txt";
    VertexAttribute rectangle_position = VertexAttributeParser::ProcessFile(rectangle_position_filepath);
    if (rectangle_position.data.empty())
    {
        printf("[VertexAttribute] Rectangle position data empty!");
        return;
    }

    const char* rectangle_normal_filepath = "vertices/rectangle_normal.txt";
    VertexAttribute rectangle_normal = VertexAttributeParser::ProcessFile(rectangle_normal_filepath);
    if (rectangle_normal.data.empty())
    {
        printf("[VertexAttribute] Rectangle normal data empty!");
        return;
    }

    const char* rectangle_uv_filepath = "vertices/rectangle_uv.txt";
    VertexAttribute rectangle_uv = VertexAttributeParser::ProcessFile(rectangle_uv_filepath);
    if (rectangle_uv.data.empty())
    {
        printf("[VertexAttribute] Rectangle UV data empty!");
        return;
    }

    const char* volume_vertices_filepath = "vertices/volume_vertices.txt";
    VertexAttribute volume_vertices = VertexAttributeParser::ProcessFile(volume_vertices_filepath);
    if (volume_vertices.data.empty())
    {
        printf("[VertexAttribute] Volume vertices data empty!");
        return;
    }

    const char* volume_elements_filepath = "vertices/volume_elements.txt";
    VertexAttribute volume_elements = VertexAttributeParser::ProcessFile(volume_elements_filepath);
    if (volume_elements.data.empty())
    {
        printf("[VertexAttribute] Volume elements data empty!");
        return;
    }

	// Prepare 'Model' data. That's about all the necessary data that's needed
	//	for a shape to get drawn on screen.
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

	// Prepare a 'ground' object, on which
	//	the shadows cast by other models will be cast.
	Model podloga;
	podloga.PushVertexAttribute(position, 0);
	podloga.PushVertexAttribute(normal, 1);
	podloga.SetScale(glm::vec3(15.0f, 1.0f, 15.0f));
	podloga.SetTranslation(glm::vec3(0.0f, -5.0f, 0.0f));
	auto podlogaModel = podloga.GetModelMatrix();

    // Prepare a 'canvas' object, which is a simple 2D rectangle that will
    //  take up the whole screen. It'll be used to present the 'depth'
    //  texture for debugging purposes.
    Model canvas;
    canvas.PushVertexAttribute(rectangle_position, 0);
    canvas.PushVertexAttribute(rectangle_uv, 1);

    Model volume;
    volume.PushVertexAttribute(volume_vertices, 0);
    volume.SetElementArrayBuffer(volume_elements);

    // Shaders.
    const char* debugVertex = "shaders/shadow_volume/debug_rect.vs";
    const char* debugFragment = "shaders/shadow_volume/debug_rect.fs";
    Shader shaderDebug(debugVertex, debugFragment);

    const char* vertexGeometryFilepath = "shaders/shadow_volume/geometry.vs";
    const char* fragmentGeometryFilepath = "shaders/shadow_volume/geometry.fs";
    Shader shaderGeometry(vertexGeometryFilepath, fragmentGeometryFilepath);

    const char* vertexVolumeFilepath = "shaders/shadow_volume/volume.vs";
    const char* geometryVolumeFilepath = "shaders/shadow_volume/volume.gs";
    const char* fragmentVolumeFilepath = "shaders/shadow_volume/volume.fs";
    Shader shaderVolume(vertexVolumeFilepath, geometryVolumeFilepath, fragmentVolumeFilepath);

    const char* vertexLightFilepath = "shaders/shadow_volume/light.vs";
    const char* fragmentLightFilepath = "shaders/shadow_volume/light.fs";
    Shader shaderLight(vertexLightFilepath, fragmentLightFilepath);

    ShadowVolume shadowVolume = ShadowVolume(1024, 1024, 1024, 1024);
    shadowVolume.LoadShaders(&shaderGeometry, &shaderVolume, &shaderLight);
    shadowVolume.PrepareFBOandTexture();

    while (!_window->ShouldClose())
    {
        _window->Clear();
        _window->Update();

        glm::mat4 lightMVP;
        // Perform a geometry pass.
        shadowVolume.GeometryPassSetup();

        lightMVP = eye.GetProjectionMatrix() * eye.GetViewMatrix() * podlogaModel;
        shadowVolume.SetMVP_Depth(lightMVP);
        shadowVolume.SetModel_Depth(podlogaModel);
        podloga.Draw();

        lightMVP = eye.GetProjectionMatrix() * eye.GetViewMatrix() * kocka1Model;
        shadowVolume.SetMVP_Depth(lightMVP);
        shadowVolume.SetModel_Depth(kocka1Model);
        kocka1.Draw();

        lightMVP = eye.GetProjectionMatrix() * eye.GetViewMatrix() * kocka2Model;
        shadowVolume.SetMVP_Depth(lightMVP);
        shadowVolume.SetModel_Depth(kocka2Model);
        kocka2.Draw();

        // Display the Depth texture for debugging purposes.
        bool debug = false;
        if (debug)
        {
            shadowVolume.DebugPassSetup(&shaderDebug);
            canvas.Draw();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else
        {
            // Perform a volume pass.
            shadowVolume.VolumePassSetup();
            shadowVolume.SetLightPosition(lightPosition);
            shadowVolume.SetView_Volume(eye.GetViewMatrix());
            shadowVolume.SetProjection_Volume(eye.GetProjectionMatrix());

            shadowVolume.SetModel_Volume(kocka1Model);
            volume.DrawVolume();

            shadowVolume.SetModel_Volume(kocka2Model);
            volume.DrawVolume();

            shadowVolume.SetModel_Volume(podlogaModel);
            volume.DrawVolume();

            // Perform a lighting pass.
            shadowVolume.LightPassSetup();
            shadowVolume.SetGeometryTextures();

            glm::vec3 ambientColor = glm::vec3(0.2f);
            glm::vec3 diffuseColor = glm::vec3(0.8f);

            // Draw the region of the Scene that is not in shadow.
            shadowVolume.SetPointLight(lightPosition, ambientColor, diffuseColor);
            glStencilFunc(GL_EQUAL, 0x00, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            canvas.Draw();

            // Draw the region of the Scene that is in shadow.
            diffuseColor = glm::vec3(0.2f);
            shadowVolume.SetPointLight(lightPosition, ambientColor, diffuseColor);
            glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            canvas.Draw();

            shadowVolume.CopyDrawnSceneToDefaultFBO();
        }

		// Draw the parent object's (Scene.cpp) 'PostDrawHook()'. It draws
		//	a GUI on top of our scene.
		PostDrawHook();

        _window->Draw();
    }
}
