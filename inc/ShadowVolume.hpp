#pragma once

#include "glm-lib/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm-lib/gtx/transform.hpp"

#include "Shader.hpp"

class ShadowVolume
{
public:
	ShadowVolume(uint window_w, uint window_h, uint shadow_w, uint shadow_h);
	~ShadowVolume();

	void LoadShaders(const Shader* geometry, const Shader* volume, const Shader* light);
	void PrepareFBOandTexture();

	void GeometryPassSetup();
	void VolumePassSetup();
	//void DebugPassSetup(const Shader* debug);	// For the purpose of rendering the depth texture's contents.
	void LightPassSetup();
	void BlitFBO();

	void SetModel_Depth(glm::mat4 M);
	void SetMVP_Depth(glm::mat4 MVP);
	void SetReverseNor(bool flag);

	void SetModel_Volume(glm::mat4 M);
	void SetView_Volume(glm::mat4 V);
	void SetProjection_Volume(glm::mat4 P);
	void SetLightPosition(glm::vec3 pos);

	void SetGeometryTextures();
	void SetPointLight(glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor);

private:
	GLuint _fbo;
	GLuint _position_texture, _normal_texture, _color_texture, _depth_texture, _final_texture;
	
	unsigned int _window_w, _window_h;
	unsigned int _shadow_w, _shadow_h;

	const Shader* _geometry = nullptr;
	const Shader* _volume = nullptr;
	const Shader* _light = nullptr;
};