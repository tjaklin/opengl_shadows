#include "../inc/DefaultScene.hpp"

DefaultScene::DefaultScene(Window* window)
    : Scene(window)
{
}

void DefaultScene::Run() const
{
    // Prepare the scene's camera object.
	Camera eye;
	glm::vec3 eyePosition = glm::vec3(-16.6f, 15.4f, 9.2f);
	eye.SetViewMatrix(eyePosition, glm::vec3(0.0f), glm::vec3(0,1,0));
	//eye.SetPerspectiveProjectionMatrix(45.0f, 4.0f/3.0f, 1.0f, 100.0f);
    eye.SetOrthogonalProjectionMatrix(-10, 10, -20, 20, -10, 50);
	
	// Set this object as the scene's main camera object.
	// This enables it to handle movement input events.
	_window->SetCamera(&eye);
	
	// Prepare 3D shape data.
	const char* cube_position_filepath = "vertices/cube_position.txt";
	VertexAttribute position = VertexAttributeParser::ProcessFile(cube_position_filepath);
	if (position.data.empty())
    {
        // TODO: Think about changing this to use exceptions, instead
        //  of simply returning.
        printf("VertexAttribute is empty. Exiting.\n");
        return;
    }
	
	const char* cube_color_filepath = "vertices/cube_color.txt";
	VertexAttribute color = VertexAttributeParser::ProcessFile(cube_color_filepath);
	if (color.data.empty())
    {
        printf("VertexAttribute is empty. Exiting.\n");
        return;
    }

	// Prepare 'Model' data. That's about all the necessary data that's needed
	//	for a shape to get drawn on screen.
	Model kocka;
	kocka.PushVertexAttribute(position, 0);
	kocka.PushVertexAttribute(color, 1);
	kocka.SetScale(glm::vec3(1.0f));
	kocka.SetTranslation(glm::vec3(-3.0f, 3.0f,-1.0f));
	auto kockaModel = kocka.GetModelMatrix();

	// Prepare shader data.
	const char* vertexShaderFilepath = "shaders/default.vs";
	const char* fragmentShaderFilepath = "shaders/default.fs";
	Shader defaultShader(vertexShaderFilepath, fragmentShaderFilepath);
	defaultShader.Bind();
	auto shader = defaultShader.Get();

    // Main update and draw loop.
    while (!_window->ShouldClose())
    {
		_window->Clear();
		_window->Update();

		auto eyeView = eye.GetViewMatrix();
		auto eyeProjection = eye.GetProjectionMatrix();

		// Forward View and Projection matrix data to the shader.
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &eyeView[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &eyeProjection[0][0]);

		// Position and draw the object.
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &kockaModel[0][0]);
		kocka.Draw();	

		// Display Window's framebuffer.
        _window->Draw();
    }
}