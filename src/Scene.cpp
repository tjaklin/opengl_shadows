#include "../inc/Scene.hpp"
#include "stb_image.h"

#include <cstdio>

Scene::Scene(Window* window)
    : _window(window)
{
}

void Scene::PreRunInitialization() const
{
    // PreRunInitialization. Perform initializations here,
    //  before entering Run's main loop.
}

void Scene::Run() const
{
    // PreRunInitialization();
    //while (true)
    {
        // Run's main loop. Draw stuf here.
        // PostDrawHook();
    }
}

void Scene::PostDrawHook() const
{
    // PostDrawHook called. Do additional draws,
    //  like GUI draws.

	const char* rectangle_position_filepath = "vertices/rectangle_position.txt";
	VertexAttribute rectangle_position = VertexAttributeParser::ProcessFile(rectangle_position_filepath);
	if (rectangle_position.data.empty())
    {
        printf("rectangle_position is empty. Exiting.\n");
        return;
    }

	const char* rectangle_uv_filepath = "vertices/rectangle_uv.txt";
	VertexAttribute rectangle_uv = VertexAttributeParser::ProcessFile(rectangle_uv_filepath);
	if (rectangle_uv.data.empty())
    {
        printf("rectangle_uv is empty. Exiting.\n");
        return;
    }

	Model GUI;
	GUI.PushVertexAttribute(rectangle_position, 0);
	GUI.PushVertexAttribute(rectangle_uv, 1);
	GUI.SetScale(glm::vec3(0.9f, 0.1f, 0.0f));
	GUI.SetTranslation(glm::vec3(0.0f, 0.8f, 0.0f));
	auto guiModel = GUI.GetModelMatrix();

    const char* vertexGUI = "shaders/rect.vs";
	const char* fragmentGUI = "shaders/rect.fs";
	Shader guiShader(vertexGUI, fragmentGUI);

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
    else
    {
        printf("[PostDrawHook] Cannot load guiImage.\n");
        return;
    }

    // Draw GUI model to display info to the user.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gui_texture);

    guiShader.Bind();
    glUniformMatrix4fv(glGetUniformLocation(guiShader.Get(), "model"), 1, GL_FALSE, &guiModel[0][0]);
    glUniform1i(glGetUniformLocation(guiShader.Get(), "texture_image"), 0);

    GUI.Draw();

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &gui_texture);
}