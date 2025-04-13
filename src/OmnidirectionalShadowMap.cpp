#include "../inc/OmnidirectionalShadowMap.hpp"

#include <cstdio>

OmnidirectionalShadowMap::OmnidirectionalShadowMap(int shadow_W, int shadow_H, int window_W, int window_H)
	: _shadow_width(shadow_W), _shadow_height(shadow_H)
	, _window_width(window_W), _window_height(window_H)
{
	;
}

OmnidirectionalShadowMap::~OmnidirectionalShadowMap()
{
	// If '_depth_texture' was ever valid and / or used
	if (_depth_texture != 255)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glDeleteTextures(1, &_depth_texture);
	}

	// If '_fbo' was ever valid and / or used
	if (_fbo != 255)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &_fbo);
	}
}

void OmnidirectionalShadowMap::PrepareFBOandTexture()
{
	glGenTextures(1, &_depth_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _depth_texture);
	
	for (GLuint i=0; i<6; ++i)
	{
		// NOTE: What are all of the available 'GL_TEXTURE_CUBE_MAP_*' targets ?
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			_shadow_width, _shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depth_texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("[OmnidirectionalShadowMap] glCheckFramebufferStatus error.\n");
		return;
	}

	glDrawBuffer(GL_FALSE);
	glReadBuffer(GL_FALSE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmnidirectionalShadowMap::LoadShaders(const Shader* depth, const Shader* light)
{
	_depth = depth;
	_light = light;
}

void OmnidirectionalShadowMap::FirstPassSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glViewport(0, 0, _shadow_width, _shadow_height);
	glClear(GL_DEPTH_BUFFER_BIT);
	_depth->Bind();

	glCullFace(GL_FRONT);
}

void OmnidirectionalShadowMap::SecondPassSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _window_width, _window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_light->Bind();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glCullFace(GL_BACK);
}

void OmnidirectionalShadowMap::SetLightPosition_Depth(glm::vec3 lightPos)
{
	glUniform3fv(glGetUniformLocation(_depth->Get(), "light_position"), 1, &lightPos[0]);
}

void OmnidirectionalShadowMap::SetLightPosition_Light(glm::vec3 lightPos)
{
	glUniform3fv(glGetUniformLocation(_light->Get(), "light_position"), 1, &lightPos[0]);
}

void OmnidirectionalShadowMap::SetModelMatrix_Depth(glm::mat4 Model)
{
	glUniformMatrix4fv(glGetUniformLocation(_depth->Get(), "model"), 1, GL_FALSE, &Model[0][0]);
}

void OmnidirectionalShadowMap::SetModelMatrix_Light(glm::mat4 Model)
{
	glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "model"), 1, GL_FALSE, &Model[0][0]);
}

void OmnidirectionalShadowMap::SetFarPlane_Depth(float far)
{
	glUniform1f(glGetUniformLocation(_depth->Get(), "far_plane"), far);
}

void OmnidirectionalShadowMap::SetFarPlane_Light(float far)
{
	glUniform1f(glGetUniformLocation(_light->Get(), "far_plane"), far);
}

void OmnidirectionalShadowMap::SetShadowMatrices(std::vector<glm::mat4> shadowMatrices)
{
	for (GLuint i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv(glGetUniformLocation(_depth->Get(), 
			("lightVP[" + std::to_string(i) + "]").c_str()), 
			1, GL_FALSE, &shadowMatrices[i][0][0]);
	}
}

void OmnidirectionalShadowMap::SetViewMatrix(glm::mat4 View)
{
	glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "view" ), 1, GL_FALSE, &View[0][0]);
}

void OmnidirectionalShadowMap::SetProjectionMatrix(glm::mat4 Proj)
{
	glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "projection"), 1, GL_FALSE, &Proj[0][0]);
}

void OmnidirectionalShadowMap::SetDepthTexture()
{
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _depth_texture);
	glUniform1i(glGetUniformLocation(_light->Get(), "shadow_map"), 7);
}