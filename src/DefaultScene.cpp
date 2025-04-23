#include "DefaultScene.hpp"

DefaultScene::DefaultScene(Window* window)
    : Scene(window)
{
}

void DefaultScene::Run() const
{
    // Prepare the scene's camera object.
	Camera eye;
	glm::vec3 eyePosition = glm::vec3(-16.6f, 10.4f, 9.2f);
	eye.SetViewMatrix(eyePosition, glm::vec3(0.0f), glm::vec3(0,1,0));
	eye.SetPerspectiveProjectionMatrix(45.0f, 4.0f/3.0f, 1.0f, 100.0f);
	
	// Set this object as the scene's main camera object.
	// This enables it to handle movement input events.
	_window->SetCamera(&eye);
	
	// Prepare 3D shape data.
	const char* cube_position_filepath = "vertices/cube_position.txt";
	VertexAttribute position = VertexAttributeParser::ProcessFile(cube_position_filepath);
	if (position.data.empty())
    {
        printf("VertexAttribute 'position' is empty. Exiting.\n");
        return;
    }
	
	const char* cube_color_filepath = "vertices/cube_color.txt";
	VertexAttribute color = VertexAttributeParser::ProcessFile(cube_color_filepath);
	if (color.data.empty())
    {
        printf("VertexAttribute 'color' is empty. Exiting.\n");
        return;
    }

	// Prepare 'Model' data. That's about all the necessary data that's needed
	//	for a shape to get drawn on screen.
	Model kocka;
	kocka.PushVertexAttribute(position, 0);
	kocka.PushVertexAttribute(color, 1);
	auto kockaModel = kocka.GetModelMatrix();

	Model podloga;
	podloga.PushVertexAttribute(position, 0);
	podloga.PushVertexAttribute(color, 1);
	podloga.SetScale(glm::vec3(10.0f, 0.5f, 10.0f));
	podloga.SetTranslation(glm::vec3(0.0f, -5.0f, 0.0f));
	auto podlogaModel = podloga.GetModelMatrix();

	// Prepare shader data.
	const char* vertexShaderFilepath = "shaders/default.vs";
	const char* fragmentShaderFilepath = "shaders/default.fs";
	Shader defaultShader(vertexShaderFilepath, fragmentShaderFilepath);

    // Main update and draw loop.
    while (!_window->ShouldClose())
    {
		_window->Clear();
		_window->Update();

		auto eyeView = eye.GetViewMatrix();
		auto eyeProjection = eye.GetProjectionMatrix();

		// Forward View and Projection matrix data to the shader.
		defaultShader.Bind();

		glUniformMatrix4fv(glGetUniformLocation(defaultShader.Get(), "view"), 1, GL_FALSE, &eyeView[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(defaultShader.Get(), "projection"), 1, GL_FALSE, &eyeProjection[0][0]);

		// Position and draw the object.
		glUniformMatrix4fv(glGetUniformLocation(defaultShader.Get(), "model"), 1, GL_FALSE, &podlogaModel[0][0]);
		podloga.Draw();

		glUniformMatrix4fv(glGetUniformLocation(defaultShader.Get(), "model"), 1, GL_FALSE, &kockaModel[0][0]);
		kocka.Draw();

		// Draw the parent object's (Scene.cpp) 'PostDrawHook()'. It draws
		//	a GUI on top of our scene.
		PostDrawHook();

		// Display Window's framebuffer.
        _window->Draw();
    }
}