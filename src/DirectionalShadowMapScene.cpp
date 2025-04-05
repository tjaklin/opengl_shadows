#include "../inc/DirectionalShadowMapScene.hpp"

DirectionalShadowMapScene::DirectionalShadowMapScene(Window* window)
    : Scene(window)
{
}

void DirectionalShadowMapScene::Run() const
{
    // Prepare the scene's main Camera object
	Camera eye;
	glm::vec3 eyePosition = glm::vec3(-3.0f, 15.0f, -1.0f);
	eye.SetViewMatrix(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0,1,0));
	eye.SetPerspectiveProjectionMatrix(45.0f, 4.0f/3.0f, 1.0f, 100.0f);

    // Set the scene's directional light source Camera object.
    Camera light;
    glm::vec3 lightPosition = glm::vec3(-16.6f, 15.4f, 9.2f);
    light.SetViewMatrix(lightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0,1,0));
    light.SetOrthogonalProjectionMatrix(-10, 10, -20, 20, -10, 50);

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

    const char* rectangle_uv_filepath = "vertices/rectangle_uv.txt";
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
	DirectionalShadowMap shadowMap(1024, 1024, 1024, 1024);
	shadowMap.LoadShaders(&shadowMapDepth, &shadowMapLight);
	shadowMap.PrepareFBOandTexture();

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
        //  Then draw the scene's object.
        auto light_vp_matrix = light.GetProjectionMatrix() * light.GetViewMatrix();
        shadowMap.SetLightMvpMatrix(light_vp_matrix * kockaModel);
        kocka.Draw();

        shadowMap.SetLightMvpMatrix(light_vp_matrix * podlogaModel);
        podloga.Draw();

        bool debug_pass = false;
        // Render the depth_texture to a 2D rectangle.
        if (debug_pass)
        {
            shadowMap.DebugPassSetup(&debugShader);
            canvas.Draw();
        }
        else
        {
            // Second stage. Draw the scene normally.
            // Connect all the 'uniform' data to be sent to shaders.
            shadowMap.SecondPassSetup();
            shadowMap.SetViewMatrix(eye.GetViewMatrix());
            shadowMap.SetProjectionMatrix(eye.GetProjectionMatrix());
            shadowMap.SetLightDirection(lightPosition); // fragment shader only
            
            // TODO: There is a variation of the ShadowMapping technique implementation
            //  which makes use of a 'biasMatrix' to further transform the
            //  'lightVP' (or 'lightBiasVP') that get's sent to the shader for
            //  calculation of fragment's z value from the light's perspective.
            //  Read up on the benefits of using said variation and consider
            //  implementing it.
            /*
                Example 'biasMatrix'.
                glm::mat4 biasMatrix = glm::mat4(
                    0.5, 0.0, 0.0, 0.0,
                    0.0, 0.5, 0.0, 0.0,
                    0.0, 0.0, 0.5, 0.0,
                    0.5, 0.5, 0.5, 1.0
                );
            */
            glm::mat4 light_vp_matrix = light.GetProjectionMatrix() * light.GetViewMatrix();

            // Set data and draw the 'kocka' model.
            shadowMap.SetModelMatrix(kockaModel);
            shadowMap.SetLightMvpMatrix(light_vp_matrix * kockaModel);
            kocka.Draw();

            // Set data and draw the 'podloga' model.
            shadowMap.SetModelMatrix(podlogaModel);
            shadowMap.SetLightMvpMatrix(light_vp_matrix * podlogaModel);
            podloga.Draw();
        }
        
        _window->Draw();
    }

}