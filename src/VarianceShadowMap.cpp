#include "../inc/VarianceShadowMap.hpp"

VarianceShadowMap::VarianceShadowMap(uint window_w, uint window_h, uint shadow_w, uint shadow_h, uint blur_w, uint blur_h)
    : _window_w(window_w), _window_h(window_h)
    , _shadow_w(shadow_w), _shadow_h(shadow_h)
    , _blur_w(blur_w), _blur_h(blur_h)
{}

VarianceShadowMap::~VarianceShadowMap()
{
	// If '_depth/blur_texture' was ever valid and / or used
	if (_depth_texture != 255)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &_depth_texture);
	}
    if (_blur_texture != 255)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &_blur_texture);
	}

	// If '_depth/blur_fbo' was ever valid and / or used
    if (_depth_fbo != 255)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &_depth_fbo);
	}
	if (_blur_fbo != 255)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &_blur_fbo);
	}
}

void VarianceShadowMap::PrepareDepthFBOandTexture()
{
	glGenTextures(1, &_depth_texture);
	glBindTexture(GL_TEXTURE_2D, _depth_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F,
        _shadow_w,
        _shadow_h,
        0, GL_RGBA, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    const float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	glGenFramebuffers(1, &_depth_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _depth_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
        GL_TEXTURE_2D, _depth_texture, 0);

	if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
    {
		printf("[VarianceShadowMap] glCheckFramebufferStatus error.\n");
		return;
    }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void VarianceShadowMap::PrepareBlurFBOandTexture()
{
	glGenTextures(1, &_blur_texture);
	glBindTexture(GL_TEXTURE_2D, _blur_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F,
        _blur_w, _blur_h,
        0, GL_RGBA, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    const float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	glGenFramebuffers(1, &_blur_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _blur_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _blur_texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("[VarianceShadowMap] glCheckFramebufferStatus error.\n");
        return;
    }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void VarianceShadowMap::LoadShaders(const Shader* depth, const Shader* blur, const Shader* light)
{
    _depth = depth;
    _blur = blur;
    _light = light;
}

void VarianceShadowMap::FirstPassSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _depth_fbo);
    glViewport(0, 0, _shadow_w, _shadow_h);
    glColorMask(GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _depth->Bind();

	glCullFace(GL_FRONT);
}

void VarianceShadowMap::DebugPassSetup(const Shader* debug)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _window_w, _window_h);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	debug->Bind();

	glCullFace(GL_BACK);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _depth_texture);
	glUniform1i(glGetUniformLocation(debug->Get(), "depth_map"), 3);
}

void VarianceShadowMap::BlurPassXSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _blur_fbo);
	glViewport(0, 0, _blur_w, _blur_h);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    _blur->Bind();

    glCullFace(GL_BACK);
}

void VarianceShadowMap::BlurPassYSetup()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _depth_fbo);
    glViewport(0, 0, _shadow_w, _shadow_h);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    _blur->Bind();

    glCullFace(GL_BACK);
}

void VarianceShadowMap::SecondPassSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(0, 0, _window_w, _window_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _light->Bind();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glCullFace(GL_BACK);
}

void VarianceShadowMap::SetLightMVP(glm::mat4 mvp)
{
    glUniformMatrix4fv(glGetUniformLocation(_depth->Get(), "mvp"), 1, GL_FALSE, &mvp[0][0]);
}

void VarianceShadowMap::SetFilterValueX_Blur()
{
    float x_value = 1.0f / (_window_w * 0.5f * 0.25f);
    glUniform2f(glGetUniformLocation(_blur->Get(),"scale_factor"), x_value, 0.0);
}

void VarianceShadowMap::SetFilterValueY_Blur()
{
    float y_value = 1.0f / (_window_w * 0.5f);
    glUniform2f(glGetUniformLocation(_blur->Get(),"scale_factor"), 0.0, y_value);
}

void VarianceShadowMap::SetLightDirection(glm::vec3 lightDir)
{
    glUniform3f(glGetUniformLocation(_light->Get(), "lightDir"), lightDir.x, lightDir.y, lightDir.z);
}

void VarianceShadowMap::SetModelMatrix(glm::mat4 model)
{
    glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "model"), 1, GL_FALSE, &model[0][0]);
}

void VarianceShadowMap::SetViewMatrix(glm::mat4 view)
{
    glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "view" ), 1, GL_FALSE, &view[0][0]);
}

void VarianceShadowMap::SetProjectionMatrix(glm::mat4 projection)
{
    glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "projection"), 1, GL_FALSE, &projection[0][0]);
}

void VarianceShadowMap::SetBiasedLightMVP(glm::mat4 mvp)
{
    glUniformMatrix4fv(glGetUniformLocation(_light->Get(), "biasMVP"), 1, GL_FALSE, &mvp[0][0]);
}

void VarianceShadowMap::SetDepthTexture_Light()
{
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _depth_texture);
    glUniform1i(glGetUniformLocation(_light->Get(), "shadow_map"), 3);
}

void VarianceShadowMap::SetDepthTexture_Blur(BlurPassType type)
{
    glActiveTexture(GL_TEXTURE3);

    if (type == BlurPassType::X) glBindTexture(GL_TEXTURE_2D, _depth_texture);
    else if (type == BlurPassType::Y) glBindTexture(GL_TEXTURE_2D, _blur_texture);
    
    glUniform1i(glGetUniformLocation(_blur->Get(), "shadow_map"), 3);
}
