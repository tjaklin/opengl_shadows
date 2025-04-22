#include "../inc/DefaultScene.hpp"

#include "stb_image.h"

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

	const char* rectangle_position_filepath = "vertices/rectangle_position.txt";
	VertexAttribute rectangle_position = VertexAttributeParser::ProcessFile(rectangle_position_filepath);
	if (rectangle_position.data.empty())
    {
        // TODO: Think about changing this to use exceptions, instead
        //  of simply returning.
        printf("VertexAttribute is empty. Exiting.\n");
        return;
    }

	const char* rectangle_uv_filepath = "vertices/rectangle_uv.txt";
	VertexAttribute rectangle_uv = VertexAttributeParser::ProcessFile(rectangle_uv_filepath);
	if (rectangle_uv.data.empty())
    {
        // TODO: Think about changing this to use exceptions, instead
        //  of simply returning.
        printf("VertexAttribute is empty. Exiting.\n");
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

	Model GUI;
	GUI.PushVertexAttribute(rectangle_position, 0);
	GUI.PushVertexAttribute(rectangle_uv, 1);
	GUI.SetScale(glm::vec3(0.9f, 0.1f, 0.0f));
	GUI.SetTranslation(glm::vec3(0.0f, 0.8f, 0.0f));
	auto guiModel = GUI.GetModelMatrix();

	// Prepare shader data.
	const char* vertexGUI = "shaders/rect.vs";
	const char* fragmentGUI = "shaders/rect.fs";
	Shader guiShader(vertexGUI, fragmentGUI);

	const char* vertexShaderFilepath = "shaders/default.vs";
	const char* fragmentShaderFilepath = "shaders/default.fs";
	Shader defaultShader(vertexShaderFilepath, fragmentShaderFilepath);

	// Try to load an image from a PNG file.
	stbi_set_flip_vertically_on_load(true);
	int width, height, colorChannels;
	unsigned char* guiImage = stbi_load("images/gui_info.png", &width, &height, &colorChannels, STBI_rgb_alpha);
	GLuint gui_texture;
	if (guiImage)
	{
		printf("[guiImage] Loaded an image of dimensions %d, %d\n", width, height);
		glGenTextures(1, &gui_texture);
		glBindTexture(GL_TEXTURE_2D, gui_texture);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, guiImage);

		stbi_image_free(guiImage);
	}

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

		// Draw GUI model to display info to the user.
		guiShader.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gui_texture);

		glUniformMatrix4fv(glGetUniformLocation(guiShader.Get(), "model"), 1, GL_FALSE, &guiModel[0][0]);
		glUniform1i(glGetUniformLocation(guiShader.Get(), "texture_image"), 0);

		glEnable(GL_BLEND);
		GUI.Draw();
		glDisable(GL_BLEND);

		// Display Window's framebuffer.
        _window->Draw();
    }
}