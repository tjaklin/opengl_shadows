#pragma once

#include "glm-lib/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm-lib/gtx/transform.hpp"

#include "Shader.hpp"

enum BlurPassType { X = 1, Y = 0 };

class VarianceShadowMap
{
public:
    VarianceShadowMap(uint window_w, uint window_h, uint shadow_w, uint shadow_h);
    ~VarianceShadowMap();

    void FirstPassSetup();
    //void DebugPassSetup();    // TODO: Add this later !
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

    // TODO: Remove '_shadow_factor' and use '_shadow_w/h' instead.
    //  Also, add '_blur_w/h' instead of _blur_factor.
    const float _blur_factor = 1.0f;
	const float _shadow_factor = 1.0f;

    const Shader* _depth = nullptr;
    const Shader* _blur = nullptr;
    const Shader* _light = nullptr;
};