#pragma once

#include "glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/transform.hpp"

#include "Shader.hpp"

class DirectionalShadowMap
{
public:
	DirectionalShadowMap(unsigned int shadow_W, unsigned int shadow_H,
		unsigned int window_W, unsigned int window_H);
	~DirectionalShadowMap();
	
	void LoadShaders(const Shader* depth, const Shader* light);
	void PrepareFBOandTexture();

	void FirstPassSetup();
	void DebugPassSetup(const Shader* debug);	// For the purpose of rendering the depth texture's contents.
	void SecondPassSetup();

	void SetModelMatrix(glm::mat4 m);
	void SetViewMatrix(glm::mat4 v);
	void SetProjectionMatrix(glm::mat4 p);

	void SetLightDirection(glm::vec3 direction);
	void SetLightMvpMatrix(glm::mat4 mvp);

private:
	unsigned int _shadow_width, _shadow_height;
	unsigned int _window_width, _window_height;

	GLuint _fbo = 255;	// '255' means invalid value.
	GLuint _depth_texture = 255;	// '255' means invalid value.

	const Shader* _depth = nullptr;
	const Shader* _light = nullptr;
};