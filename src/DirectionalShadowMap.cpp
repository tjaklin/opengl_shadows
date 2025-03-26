#include "../inc/DirectionalShadowMap.hpp"

#include <cstdio>

DirectionalShadowMap::DirectionalShadowMap(uint shadow_w, uint shadow_h, uint window_w,
	uint window_h)
	: _shadow_width(shadow_w), _shadow_height(shadow_h), _window_width(window_w),
		_window_height(window_h), _fbo(255), _depth_texture(255)
{
	;
}

DirectionalShadowMap::~DirectionalShadowMap()
{
	glDeleteFramebuffers(1, &_fbo);
	_fbo = 255; // 255 means invalid value.
	glDeleteTextures(1, &_depth_texture);
	_depth_texture = 255; // 255 means invalid value.
}

void DirectionalShadowMap::PrepareFBOandTexture()
{
	glGenFramebuffers(1, &_fbo);
	glGenTextures(1, &_depth_texture);

	glBindTexture(GL_TEXTURE_2D, _depth_texture);
	
	// NOTE: Documentation states that 'width' and 'height' values that
	//	are supported are upwards of 1024 texels. What happens if i use a
	//	smaller value (eg. width that's equal to window_width of 960) ?
	// In this call, i pass NULL to the 'data' argument, because
	//	i mean to use the texture for output, instead of input.
	// Play around with 'internalFormat' argument's precision (24, 32, 16, ...)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, _shadow_width, _shadow_height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	
	// Set the values of the default sampler, that's embedded into the
	//	texture object currently bound to 'GL_TEXTURE_2D' target.
	//	I'm not using a custom sampler object here.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// TODO: Figure out what 'GL_TEXTURE_COMPARE_*' parameters mean.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	// NOTE: Mozda je moguce da mozda i ne moram koristiti zaseban Framebuffer
	//	objekt da bi postigel crtanje 'dubine' u teksturu. Ali moozda, trebala
	//	bi se ova hrabra tvrdnja provjeriti uz pomoc knjige ("Writing to Textures").
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depth_texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		glDeleteFramebuffers(1, &_fbo);
		_fbo = 255; // 255 means invalid value.
		glDeleteTextures(1, &_depth_texture);
		_depth_texture = 255; // 255 means invalid value.

		printf("[CHECK_FB_STATUS]: An error happened! :)\n");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalShadowMap::LoadShaders(const Shader* depth, const Shader* light)
{
	// TODO: Add some error checking for arguments, if necessary.
	_depth = depth;
	_light = light;
}

void DirectionalShadowMap::FirstPassSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glViewport(0, 0, _shadow_width, _shadow_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	_depth->Bind();

	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
}

void DirectionalShadowMap::SecondPassSetup()
{
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, _window_width, _window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_light->Bind();
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
   glUniformMatrix4fv(glGetUniformLocation(_depth->Get(), "lMVP"), 1, GL_FALSE, &mvp[0][0]);
}

void DirectionalShadowMap::SetBiasedLightMvpMatrix(glm::mat4 biased)
{
   glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "lBiasMVP"), 1, GL_FALSE, &biased[0][0]);
}

void DirectionalShadowMap::SetIsPCF(bool isPCF)
{
   glUniform1i(glGetUniformLocation(_light->Get(), "isPCF"), (int)isPCF);
}

void DirectionalShadowMap::BindDepthTexture() const
{
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, _depth_texture);
   // The line below cannot be commented out, because i added
   //	'layout (binding=0)' to shader code AND because i use GLSL below v430.
   glUniform1i(glGetUniformLocation(_light->Get(), "shadowMap"), 0);
}

// TODO: Not used.
void DirectionalShadowMap::UnbindDepthTexture() const
{
   glBindTexture(GL_TEXTURE_2D, 0);
}