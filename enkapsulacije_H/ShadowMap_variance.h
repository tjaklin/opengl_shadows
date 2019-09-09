#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "loadShader.h"

class ShadowMap_variance{

private:
	int WINDOW_WIDTH  = 0;
	int WINDOW_HEIGHT = 0;
	float BLUR_COEF   = 1.0f;
	float SHADOW_COEF = 1.0f;

	GLuint mFBO_depth;
	GLuint mFBO_blur;
	GLuint mDepthTexture;
	GLuint mBlurTexture;

	GLuint shader_depthID;
	GLuint shader_blurID;
	GLuint shader_lightingID;

	bool loadShaders();
	bool fillFBO_depth();
	bool fillFBO_blur();

public:
	bool Init( int window_W, int window_H );

	void depthPassSetup();
	void blurXPassSetup();
	void blurYPassSetup();
	void lightPassSetup();

	void setLightMVP( glm::mat4 lMVP);

	void setFilterValX();
	void setFilterValY();
	void setDepthTextureB( bool isXpass );

	void setLightDir( glm::vec3 lightDir);
	void setModel	( glm::mat4 Model);
	void setView	( glm::mat4 View );
	void setProj	( glm::mat4 Proj );
	void setBiasedLightMVP( glm::mat4 biasedlMVP);
	void setDepthTextureL();
};