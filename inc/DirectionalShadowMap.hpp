#pragma once

#include "glm-lib/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm-lib/gtx/transform.hpp"

#include "Shader.hpp"

class DirectionalShadowMap
{
public:
	DirectionalShadowMap(uint shadow_W, uint shadow_H, uint window_W, uint window_H);
	~DirectionalShadowMap();
	
	void LoadShaders(const Shader* depth, const Shader* light);
	void PrepareFBOandTexture();

	void FirstPassSetup();
	void SecondPassSetup();

	void SetModelMatrix(glm::mat4 m);
	void SetViewMatrix(glm::mat4 v);
	void SetProjectionMatrix(glm::mat4 p);

	void SetLightDirection(glm::vec3 direction);
	void SetLightMvpMatrix(glm::mat4 mvp);

	void SetBiasedLightMvpMatrix(glm::mat4 biased);

	void SetIsPCF(bool isPCF);

	void BindDepthTexture() const;
	void UnbindDepthTexture() const;

private:
	uint _shadow_width;
	uint _shadow_height;
	uint _window_width;
	uint _window_height;

	GLuint _fbo;
	GLuint _depth_texture;

	const Shader* _depth = nullptr;
	const Shader* _light = nullptr;
};