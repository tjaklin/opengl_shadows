#pragma once

#include "glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/transform.hpp"

#include "Shader.hpp"

enum BlurPassType { X = 1, Y = 0 };

class VarianceShadowMap
{
public:
    VarianceShadowMap(uint window_w, uint window_h, uint shadow_w, uint shadow_h, uint blur_w, uint blur_h);
    ~VarianceShadowMap();

    void FirstPassSetup();
    void DebugPassSetup(const Shader* debug);
    void BlurPassXSetup();
    void BlurPassYSetup();
    void SecondPassSetup();

	void SetLightMVP(glm::mat4 mvp);
	void SetBiasedLightMVP(glm::mat4 mvp);

	void SetFilterValueX_Blur();
	void SetFilterValueY_Blur();
	void SetDepthTexture_Blur(BlurPassType type);
	void SetDepthTexture_Light();

	void SetLightDirection(glm::vec3 light);
	void SetModelMatrix(glm::mat4 model);
	void SetViewMatrix(glm::mat4 view);
	void SetProjectionMatrix(glm::mat4 projection);

public:
    void LoadShaders(const Shader* depth, const Shader* blur, const Shader* light);
    void PrepareDepthFBOandTexture();
    void PrepareBlurFBOandTexture();

private:
    GLuint _depth_fbo = 255, _blur_fbo = 255;
    GLuint _depth_texture = 255, _blur_texture = 255;

    unsigned int _window_w, _window_h;
    unsigned int _shadow_w, _shadow_h;
    unsigned int _blur_w, _blur_h;

    const Shader* _depth = nullptr;
    const Shader* _blur = nullptr;
    const Shader* _light = nullptr;
};