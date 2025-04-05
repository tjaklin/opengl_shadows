#pragma once

#include "glm-lib/glm.hpp"
#include "Shader.hpp"

#include <vector>

class OmnidirectionalShadowMap
{
public:
	OmnidirectionalShadowMap(int shadow_W, int shadow_H, int window_W, int window_H);
	~OmnidirectionalShadowMap();

	void FirstPassSetup();
//	void DebugPassSetup();		Add later !
	void SecondPassSetup();

	void setModelFP(glm::mat4 Model);
	void setFarPlaneFP(float far);
	void setModelSP(glm::mat4 Model);
	void setFarPlaneSP(float far);

	void setView(glm::mat4 View);
	void setProj(glm::mat4 Proj);

	void setLightPos(glm::vec3 lightPos);
	void setPointLight(glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse);

	void setReverseNor(bool areReversed);

	void setDepthTexture(uint offset = 1);
	void setShadowMatrices(std::vector<glm::mat4> shadowMatrices);

public:
	void PrepareFBOandTexture();
	void LoadShaders(const Shader* depth, const Shader* light);

private:
	uint _shadow_width, _shadow_height;
	uint _window_width, _window_height;

	GLuint _fbo = 255;	// '255' means invalid value.
	GLuint _depth_texture = 255;	// '255' means invalid value.

	const Shader* _depth = nullptr;
	const Shader* _light = nullptr;
};