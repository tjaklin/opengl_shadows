#include "DirectionalShadowMap.hpp"

#include <cstdio>

DirectionalShadowMap::DirectionalShadowMap(uint shadow_w, uint shadow_h, uint window_w,
	uint window_h)
	: _shadow_width(shadow_w), _shadow_height(shadow_h)
	, _window_width(window_w), _window_height(window_h)
{
}

DirectionalShadowMap::~DirectionalShadowMap()
{
	// If '_depth_texture' was ever valid and / or used
	if (_depth_texture != 255)
	{
		glDeleteTextures(1, &_depth_texture);
		_depth_texture = 255;
	}

	// If '_fbo' was ever valid and / or used
	if (_fbo != 255)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &_fbo);
	}
}

void DirectionalShadowMap::PrepareFBOandTexture()
{
	glGenFramebuffers(1, &_fbo);
	glGenTextures(1, &_depth_texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _depth_texture);
	
	// NOTE: Documentation states that 'width' and 'height' values that
	//	are supported are upwards of 1024 texels. What happens if i use a
	//	smaller value (eg. width that's equal to window_width of 960) ?
	// In this call, i pass NULL to the 'data' argument, because
	//	i mean to use the texture for output, instead of input.
	//	NOTE: What are the benefits of using bigger 'internalFormat'
	//	precision (16, 24, 32) ?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _shadow_width, _shadow_height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	
	// Set the values of the default sampler, that's embedded into the
	//	texture object currently bound to 'GL_TEXTURE_2D' target.
	//	I'm not using a custom sampler object here.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// NOTE: Figure out what 'GL_TEXTURE_COMPARE_*' parameters mean.
	//	These commented configurations prevented me from getting a meaningful
	//	depth texture ... Why were they even here in the first place ?
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		glDeleteFramebuffers(1, &_fbo);
		_fbo = 255; // 255 means invalid value.
		glDeleteTextures(1, &_depth_texture);
		_depth_texture = 255; // 255 means invalid value.

		printf("[DirectionalShadowMap] glCheckFramebufferStatus error.\n");
		return;
	}

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalShadowMap::LoadShaders(const Shader* depth, const Shader* light)
{
	// NOTE: Add some error checking for arguments, if necessary.
	_depth = depth;
	_light = light;
}

void DirectionalShadowMap::FirstPassSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glViewport(0, 0, _shadow_width, _shadow_height);
	glClear(GL_DEPTH_BUFFER_BIT);
	// When rendering into a depthmap, we want to draw only
	//	the back faces of objects. By doing this, we ensure
	//	that, once the scene is drawn in the second step,
	//	the objects' back face remains lit and not in shadow.
	//	If we don't cull the front face, the objects' back
	//	face will be considered in shadow during the second
	//	step.
	glCullFace(GL_FRONT);
	_depth->Bind();
}

void DirectionalShadowMap::DebugPassSetup(const Shader* debug)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _window_width, _window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	debug->Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _depth_texture);
	glUniform1i(glGetUniformLocation(debug->Get(), "depthMap"), 0);
}

void DirectionalShadowMap::SecondPassSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _window_width, _window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Restore culling of the back faces, for the
	//	second rendering pass.
	glCullFace(GL_BACK);

	_light->Bind();

	glActiveTexture(GL_TEXTURE0);
   	glBindTexture(GL_TEXTURE_2D, _depth_texture);
	// The line below cannot be commented out, because i added
	//	'layout (binding=0)' to shader code AND because i use GLSL below v430.
	glUniform1i(glGetUniformLocation(_light->Get(), "shadowMap"), 0);
}

void DirectionalShadowMap::SetModelMatrix(glm::mat4 m)
{
   glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "model"), 1, GL_FALSE, &m[0][0]);
}

void DirectionalShadowMap::SetViewMatrix(glm::mat4 v)
{
   glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "view" ), 1, GL_FALSE, &v[0][0]);
}

void DirectionalShadowMap::SetProjectionMatrix(glm::mat4 p)
{
   glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "projection"), 1, GL_FALSE, &p[0][0]);
}

void DirectionalShadowMap::SetLightDirection(glm::vec3 direction)
{
   glUniform3fv(glGetUniformLocation(_light->Get(), "lightDir"), 1, &direction[0]);
}

void DirectionalShadowMap::SetLightMvpMatrix(glm::mat4 mvp)
{
   glUniformMatrix4fv(glGetUniformLocation(_depth->Get(), "lightMVP"), 1, GL_FALSE, &mvp[0][0]);
   glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "lightMVP"), 1, GL_FALSE, &mvp[0][0]);
}
