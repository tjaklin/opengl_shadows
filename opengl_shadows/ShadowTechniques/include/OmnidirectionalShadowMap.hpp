#pragma once

#include "glm.hpp"
#include "Shader.hpp"

#include <vector>

class OmnidirectionalShadowMap
{
public:
	OmnidirectionalShadowMap(unsigned int shadow_W, unsigned int shadow_H, unsigned int window_W, unsigned int window_H);
	~OmnidirectionalShadowMap();

	void FirstPassSetup();
	//void DebugPassSetup();	// NOTE: Add later !
	void SecondPassSetup();

	void SetModelMatrix_Depth(glm::mat4 Model);
	void SetModelMatrix_Light(glm::mat4 Model);
	void SetViewMatrix(glm::mat4 View);
	void SetProjectionMatrix(glm::mat4 Proj);

	void SetLightPosition_Depth(glm::vec3 lightPos);
	void SetLightPosition_Light(glm::vec3 lightPos);

	void SetFarPlane_Depth(float far);
	void SetFarPlane_Light(float far);

	void SetDepthTexture();
	void SetShadowMatrices(std::vector<glm::mat4> shadowMatrices);

public:
	void PrepareFBOandTexture();
	void LoadShaders(const Shader* depth, const Shader* light);

private:
	unsigned int _shadow_width, _shadow_height;
	unsigned int _window_width, _window_height;

	GLuint _fbo = 255;	// '255' means invalid value.
	GLuint _depth_texture = 255;	// '255' means invalid value.

	const Shader* _depth = nullptr;
	const Shader* _light = nullptr;
};