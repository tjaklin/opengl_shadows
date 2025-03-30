#pragma once

#include "glm-lib/glm.hpp"
#include "Shader.hpp"

class DirectionalShadowMap
{
public:
	ShadowMap_dir(uint shadow_W, uint shadow_H, uint window_W, uint window_H );
	~ShadowMap_dir(); // TODO: Implement later !

	bool LoadShaders(const Shader* depth, const Shader* light);
	void firstPassSetup();
	void secondPassSetup();

	void setLightDir( glm::vec3 lightDir);
	void setLightMVP( glm::mat4 lMVP);

	void setBiasedLightMVP( glm::mat4 biasedlMVP);

	void setModel	( glm::mat4 Model);
	void setView	( glm::mat4 View );
	void setProj	( glm::mat4 Proj );

	void setIsPCF	( bool isPCF );
	void setDepthTexture( uint offset = 1);

private:
	bool fillFBO();

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