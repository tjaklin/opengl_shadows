#include "VarianceShadowMapScene.hpp"

VarianceShadowMapScene::VarianceShadowMapScene(Window* window)
    : Scene(window)
{}

void VarianceShadowMapScene::Run() const
{
    // Prepare the scene's main Camera object
	Camera eye;
	glm::vec3 eyePosition = glm::vec3(0.0f, 5.0f, 20.0f);
    glm::vec3 eyeLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	eye.SetViewMatrix(eyePosition, eyeLookAt, glm::vec3(0,1,0));
	eye.SetPerspectiveProjectionMatrix(45.0f, 4.0f/3.0f, 1.0f, 100.0f);

    // Set the scene's directional light source Camera object.
    Camera light;
    glm::vec3 lightPosition = glm::vec3(10.0f, 15.0f, 10.0f);
    glm::vec3 lightLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
    light.SetViewMatrix(lightPosition, lightLookAt, glm::vec3(0,1,0));
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
    canvas.PushVertexAttribute(rectangle_normal, 1);
    canvas.PushVertexAttribute(rectangle_uv, 2);

	// Prepare Variance Shadow Map shader data.
	const char* vertexShadowMapDepthFilepath = "shaders/shadow_map/variance/depth.vs";
	const char* fragmentShadowMapDepthFilepath = "shaders/shadow_map/variance/depth.fs";
	Shader shadowMapDepth(vertexShadowMapDepthFilepath, fragmentShadowMapDepthFilepath);

	const char* vertexShadowMapBlurFilepath = "shaders/shadow_map/variance/blur.vs";
	const char* fragmentShadowMapBlurFilepath = "shaders/shadow_map/variance/blur.fs";
	Shader shadowMapBlur(vertexShadowMapBlurFilepath, fragmentShadowMapBlurFilepath);

	const char* vertexShadowMapLightFilepath = "shaders/shadow_map/variance/light.vs";
	const char* fragmentShadowMapLightFilepath = "shaders/shadow_map/variance/light.fs";
	Shader shadowMapLight(vertexShadowMapLightFilepath, fragmentShadowMapLightFilepath);

    // Prepare default shader for 2D rectangle debugging.
    const char* vertexDebugFilepath = "shaders/shadow_map/variance/debug_rect.vs";
    const char* fragmentDebugFilepath = "shaders/shadow_map/variance/debug_rect.fs";
    Shader debugShader(vertexDebugFilepath, fragmentDebugFilepath);

	// Set up the Variance Shadow Map object.
	VarianceShadowMap shadowMap(1024, 1024, 1024, 1024, 1024, 1024);
	shadowMap.LoadShaders(&shadowMapDepth, &shadowMapBlur, &shadowMapLight);
	shadowMap.PrepareDepthFBOandTexture();
	shadowMap.PrepareBlurFBOandTexture();

    // Prepare a 'Bias' matrix.
    glm::mat4 biasMatrix =
        glm::mat4(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );

	// Main update and draw loop.
    while (!_window->ShouldClose())
    {
		_window->Clear();
		_window->Update();
		
        glm::mat4 biasLightMVP;
        glm::mat4 lightMVP;

        // First rendering pass.
        shadowMap.FirstPassSetup();

        lightMVP = light.GetProjectionMatrix() * light.GetViewMatrix() * podlogaModel;
        shadowMap.SetLightMVP(lightMVP);
        podloga.Draw();

        lightMVP = light.GetProjectionMatrix() * light.GetViewMatrix() * kocka1Model;
        shadowMap.SetLightMVP(lightMVP);
        kocka1.Draw();

        lightMVP = light.GetProjectionMatrix() * light.GetViewMatrix() * kocka2Model;
        shadowMap.SetLightMVP(lightMVP);
        kocka2.Draw();

        bool debug_pass = false;
        // Render the depth_texture to a 2D rectangle.
        if (debug_pass)
        {
            shadowMap.DebugPassSetup(&debugShader);
            canvas.Draw();
        }
        else
        {
            // 1st blur pass.
            shadowMap.BlurPassXSetup();
            shadowMap.SetFilterValueX_Blur();
            shadowMap.SetDepthTexture_Blur(BlurPassType::X);
            canvas.Draw();

            // 2nd blur pass.
            shadowMap.BlurPassYSetup();
            shadowMap.SetFilterValueY_Blur();
            shadowMap.SetDepthTexture_Blur(BlurPassType::Y);
            canvas.Draw();

            // Second rendering pass.
            shadowMap.SecondPassSetup();
            shadowMap.SetLightDirection(lightPosition);
            shadowMap.SetViewMatrix(eye.GetViewMatrix());
            shadowMap.SetProjectionMatrix(eye.GetProjectionMatrix());
            shadowMap.SetDepthTexture_Light();

            lightMVP = light.GetProjectionMatrix() * light.GetViewMatrix() * kocka1Model;
            biasLightMVP = biasMatrix * lightMVP;
            shadowMap.SetModelMatrix(kocka1Model);
            shadowMap.SetBiasedLightMVP(biasLightMVP);
            kocka1.Draw();

            lightMVP = light.GetProjectionMatrix() * light.GetViewMatrix() * kocka2Model;
            biasLightMVP = biasMatrix * lightMVP;
            shadowMap.SetModelMatrix(kocka2Model);
            shadowMap.SetBiasedLightMVP(biasLightMVP);
            kocka2.Draw();

            lightMVP = light.GetProjectionMatrix() * light.GetViewMatrix() * podlogaModel;
            biasLightMVP = biasMatrix * lightMVP;
            shadowMap.SetModelMatrix(podlogaModel);
            shadowMap.SetBiasedLightMVP(biasLightMVP);
            podloga.Draw();
        }

		// Draw the parent object's (Scene.cpp) 'PostDrawHook()'. It draws
		//	a GUI on top of our scene.
		PostDrawHook();

        _window->Draw();
    }

}