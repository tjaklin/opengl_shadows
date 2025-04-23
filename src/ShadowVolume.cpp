#include "../inc/ShadowVolume.hpp"

ShadowVolume::ShadowVolume(uint window_w, uint window_h, uint shadow_w, uint shadow_h)
	: _window_w(window_w), _window_h(window_h), _shadow_w(shadow_w), _shadow_h(shadow_h)
{
	// NOTE: Start using '_shadow_w/h' or delete them!
	(void) _shadow_w; (void) _shadow_h;
}

ShadowVolume::~ShadowVolume()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &_position_texture);
	glDeleteTextures(1, &_normal_texture);
	glDeleteTextures(1, &_color_texture);
	glDeleteTextures(1, &_depth_texture);
	glDeleteTextures(1, &_final_texture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &_fbo);
}

void ShadowVolume::LoadShaders(const Shader* geometry, const Shader* volume, const Shader* light)
{
	_geometry = geometry;
	_volume = volume;
	_light = light;
}

void ShadowVolume::PrepareFBOandTexture()
{
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	// Position
	glGenTextures(1, &_position_texture);
	glBindTexture(GL_TEXTURE_2D, _position_texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, _window_w, _window_h, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _position_texture, 0);

	// Normal
	glGenTextures(1, &_normal_texture);
	glBindTexture(GL_TEXTURE_2D, _normal_texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, _window_w, _window_h, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _normal_texture, 0);

	// Color
	glGenTextures(1, &_color_texture);
	glBindTexture(GL_TEXTURE_2D, _color_texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, _window_w, _window_h, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _color_texture, 0);

	// Depth
	glGenTextures(1, &_depth_texture);
	glBindTexture(GL_TEXTURE_2D, _depth_texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, _window_w, _window_h, 0,
		GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depth_texture, 0);

	// Final Texture
	glGenTextures(1, &_final_texture);
	glBindTexture(GL_TEXTURE_2D, _final_texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, _window_w, _window_h, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, _final_texture, 0);

	// NOTE: Should this check be performed after every 'glFramebufferTexture2D' call?
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		glDeleteFramebuffers(1, &_fbo);
		_fbo = 255; // 255 means invalid value.

		glDeleteTextures(1, &_position_texture);
		_position_texture = 255; // 255 means invalid value.
		glDeleteTextures(1, &_normal_texture);
		_normal_texture = 255; // 255 means invalid value.
		glDeleteTextures(1, &_color_texture);
		_color_texture = 255; // 255 means invalid value.
		glDeleteTextures(1, &_depth_texture);
		_depth_texture = 255; // 255 means invalid value.
		glDeleteTextures(1, &_final_texture);
		_final_texture = 255; // 255 means invalid value.

		printf("[ShadowVolume] glCheckFramebufferStatus error.\n");
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowVolume::GeometryPassSetup()
{
	glViewport(0, 0, _window_w, _window_h);

	// Clear the previous states.
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0x00, 0xFF);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_FRONT_AND_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDrawBuffer(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glDrawBuffer(GL_FRONT_AND_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);

	glDepthMask(GL_TRUE);	// Enable writing to Depth
	glEnable(GL_CULL_FACE);
	glStencilMask(0x00);	// Disable writing to Stencil

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	_geometry->Bind();
}

void ShadowVolume::DebugPassSetup(const Shader* debug)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _depth_texture);

	debug->Bind();
	glUniform1i(glGetUniformLocation(debug->Get(), "textureUv"), 0);
}

void ShadowVolume::VolumePassSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);	// Rebind custom fbo.
	glDepthMask(GL_FALSE);		// Disable writing to Depth
	glEnable(GL_DEPTH_CLAMP);	// Disable clipping of primitives that are too near & far. Instead clampem.
	glDisable(GL_CULL_FACE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Disable writing to Color(i) attachments

	glStencilMask(0xFF);	// Enable writing to Stencil
	glStencilFunc(GL_ALWAYS, 0x00, 0xFF);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	glClear(GL_STENCIL_BUFFER);
	_volume->Bind();
}

void ShadowVolume::LightPassSetup()
{
	glDepthMask(GL_TRUE);
	glDisable(GL_DEPTH_CLAMP);
	glEnable(GL_CULL_FACE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glStencilMask(0x00);	// Disable writing to Stencil
	glDrawBuffer(GL_COLOR_ATTACHMENT4);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _position_texture);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _normal_texture);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _color_texture);

	_light->Bind();
}

void ShadowVolume::CopyDrawnSceneToDefaultFBO()
{
	glStencilFunc(GL_ALWAYS, 0x00, 0xFF);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT4);

	glBlitFramebuffer(0, 0, _window_w, _window_h, 0, 0, _window_w, _window_h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void ShadowVolume::SetLightPosition(glm::vec3 pos)
{
    glUniform3f(glGetUniformLocation(_volume->Get(), "light_position"), pos.x, pos.y, pos.z);
}

void ShadowVolume::SetMVP_Depth(glm::mat4 mvp)
{
    glUniformMatrix4fv(glGetUniformLocation(_geometry->Get(), "MVP"), 1, GL_FALSE, &mvp[0][0]);
}

void ShadowVolume::SetModel_Depth(glm::mat4 m)
{
    glUniformMatrix4fv(glGetUniformLocation(_geometry->Get(), "model"), 1, GL_FALSE, &m[0][0]);
}

void ShadowVolume::SetModel_Volume(glm::mat4 m)
{
    glUniformMatrix4fv(glGetUniformLocation(_volume->Get(), "model"), 1, GL_FALSE, &m[0][0]);
}

void ShadowVolume::SetView_Volume(glm::mat4 v)
{
    glUniformMatrix4fv(glGetUniformLocation(_volume->Get(), "view"), 1, GL_FALSE, &v[0][0]);
}

void ShadowVolume::SetProjection_Volume(glm::mat4 p)
{
    glUniformMatrix4fv(glGetUniformLocation(_volume->Get(), "projection"), 1, GL_FALSE, &p[0][0]);
}

void ShadowVolume::SetGeometryTextures()
{
    glUniform1i(glGetUniformLocation(_light->Get(), "texture_position"), 2);
    glUniform1i(glGetUniformLocation(_light->Get(), "texture_normal"), 3);
    glUniform1i(glGetUniformLocation(_light->Get(), "texture_color"), 4);
}

void ShadowVolume::SetPointLight(glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor)
{
    glUniform3f(glGetUniformLocation(_light->Get(), "light_source.position"), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(_light->Get(), "light_source.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
    glUniform3f(glGetUniformLocation(_light->Get(), "light_source.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
}