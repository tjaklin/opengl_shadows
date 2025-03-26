#include "../inc/ShadowMapScene.hpp"

ShadowMapScene::ShadowMapScene(Window* window)
    : Scene(window)
{
}

void ShadowMapScene::Run() const
{
    // Prepare the scene's main Camera object
	Camera eye;
	glm::vec3 eyePosition = glm::vec3(-3.0f, 20.0f, -1.0f);
	eye.SetViewMatrix(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0,1,0));
	eye.SetPerspectiveProjectionMatrix(45.0f, 4.0f/3.0f, 1.0f, 100.0f);

    // Set the scene's directional light source Camera object.
    Camera light;
    glm::vec3 lightPosition = glm::vec3(-16.6f, 15.4f, 9.2f);
    light.SetViewMatrix(lightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0,1,0));
    int near_plane = -10, far_plane = 50;
    light.SetOrthogonalProjectionMatrix(-10, 10, -20, 20, near_plane, far_plane);

	// Set the scene's main camera object.
	// This enables it to handle movement input events.
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

    const char* rectangle_uv_filepath = "vertices/rectangle_position.txt";
    VertexAttribute rectangle_uv = VertexAttributeParser::ProcessFile(rectangle_uv_filepath);
    if (rectangle_uv.data.empty())
    {
        printf("[VertexAttribute] Rectangle UV data empty!");
        return;
    }

	// Prepare 'Model' data. That's about all the necessary data that's needed
	//	for a shape to get drawn on screen.
	Model kocka;
	kocka.PushVertexAttribute(position, 0);
	kocka.PushVertexAttribute(normal, 1);
	kocka.SetScale(glm::vec3(1.0f));
	kocka.SetTranslation(glm::vec3(-3.0f, 3.0f,-1.0f));
	auto kockaModel = kocka.GetModelMatrix();

	// Prepare a 'ground' object, on which
	//	the shadows cast by other models will be cast.
	Model podloga;
	podloga.PushVertexAttribute(position, 0);
	podloga.PushVertexAttribute(normal, 1);
	podloga.SetScale(glm::vec3(15.0f, 1.0f, 15.0f));
	podloga.SetTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
	auto podlogaModel = podloga.GetModelMatrix();

    // Prepare a 'canvas' object, which is a simple 2D rectangle that will
    //  take up the whole screen. It'll be used to present the 'depth'
    //  texture for debugging purposes.
    Model canvas;
    canvas.PushVertexAttribute(rectangle_position, 0);
    canvas.PushVertexAttribute(rectangle_uv, 1);
    canvas.SetScale(glm::vec3(1.0f));
    canvas.SetTranslation(glm::vec3(1.0f));
    auto canvasModel = canvas.GetModelMatrix();

	// Prepare Directional Shadow Map shader data.
	const char* vertexShadowMapDepthFilepath = "shaders/shadow_map/directional/sm_depth.vs";
	const char* fragmentShadowMapDepthFilepath = "shaders/shadow_map/directional/sm_depth.fs";
	Shader shadowMapDepth(vertexShadowMapDepthFilepath, fragmentShadowMapDepthFilepath);

	const char* vertexShadowMapLightFilepath = "shaders/shadow_map/directional/sm_light.vs";
	const char* fragmentShadowMapLightFilepath = "shaders/shadow_map/directional/sm_light.fs";
	Shader shadowMapLight(vertexShadowMapLightFilepath, fragmentShadowMapLightFilepath);

    // Prepare default shader for 2D rectangle debugging.
    const char* vertexDebugFilepath = "shaders/shadow_map/directional/debug_rect.vs";
    const char* fragmentDebugFilepath = "shaders/shadow_map/directional/debug_rect.fs";
    Shader debugShader(vertexDebugFilepath, fragmentDebugFilepath);

	// Set up the Directional Shadow Map object.
	//DirectionalShadowMap shadowMap(960, 540, 960, 540);
	DirectionalShadowMap shadowMap(1024, 1024, 1024, 1024);
	shadowMap.LoadShaders(&shadowMapDepth, &shadowMapLight);
	shadowMap.PrepareFBOandTexture();	// TODO: Ovo bi moglo biti prebacenu u neki cstr ?

	// Main update and draw loop.
    while (!_window->ShouldClose())
    {
		_window->Clear();
		_window->Update();

        // Get ready for the First stage of drawing.
        //	In this stage, the scene is drawn from the
        //	perspective of 'directional_camera', which
        //	has a position and it's 'looking' orientation
        //	is orientated towards the objects that need
        //	to cast shadows.			
        shadowMap.FirstPassSetup();
        // Set the shader uniform data for the First drawing stage.
        auto light_mvp_matrix = light.GetProjectionMatrix() * light.GetViewMatrix() * kockaModel;
        shadowMap.SetLightMvpMatrix(light_mvp_matrix);
        // Draw all objects that need to cast shadows.
        kocka.Draw();

        bool debug_pass = false;
        // Render the depth_texture to a 2D rectangle.
        if (debug_pass)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, 1024, 1024);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, shadowMap.GetTexture());

            debugShader.Bind();
            auto shader = debugShader.Get();
            glUniform1i(glGetUniformLocation(shader, "depthMap"), 0);
            glUniform1f(glGetUniformLocation(shader, "near_plane"), near_plane);
            glUniform1f(glGetUniformLocation(shader, "far_plane"), far_plane);
            
            canvas.Draw();
        }

        if (!debug_pass)
        {
            auto eyeV = eye.GetViewMatrix();
            auto eyeP = eye.GetProjectionMatrix();

            // Second stage. Draw the scene normally.
            // Connect all the 'uniform' data to be sent to shaders.
            shadowMap.SecondPassSetup();
            shadowMap.SetViewMatrix(eyeV);
            shadowMap.SetProjectionMatrix(eyeP);
            shadowMap.SetLightDirection(lightPosition);
            shadowMap.SetIsPCF(false);
            shadowMap.BindDepthTexture();
            
            glm::mat4 biasMVP = glm::mat4(
                0.5, 0.0, 0.0, 0.0,
                0.0, 0.5, 0.0, 0.0,
                0.0, 0.0, 0.5, 0.0,
                0.5, 0.5, 0.5, 1.0
            );
            glm::mat4 lightBiasMVP = biasMVP * light.GetProjectionMatrix() * light.GetViewMatrix();

            // Set data and draw the 'kocka' model.
            auto kockaLightBiasMVP = lightBiasMVP * kockaModel;
            shadowMap.SetModelMatrix(kockaModel);
            shadowMap.SetBiasedLightMvpMatrix(kockaLightBiasMVP);
            kocka.Draw();

            // Set data and draw the 'podloga' model.
            auto podlogaLightBiasMVP = lightBiasMVP * podlogaModel;
            shadowMap.SetModelMatrix(podlogaModel);
            shadowMap.SetBiasedLightMvpMatrix(podlogaLightBiasMVP);
            podloga.Draw();
        }

        _window->Draw();
    }

}