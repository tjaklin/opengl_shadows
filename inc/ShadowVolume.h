#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

#include "loadShader.h"

class ShadowVolume{

private:
	int SHADOW_WIDTH  = 0;
	int SHADOW_HEIGHT = 0;
	int WINDOW_WIDTH  = 0;
	int WINDOW_HEIGHT = 0;
	GLuint mFBO;
	GLuint mPosTex, mNormTex, mColorTex, mDepthTex, mFinalTex;
	GLenum* drawBuffers = NULL;

	GLuint shader_geometryID;
	GLuint shader_volumeID;
	GLuint shader_lightingID;

	bool loadShaders();
	bool fillFBO();

public:
	bool Init( int shadow_W, int shadow_H,
		   int window_W, int window_H );

	void geometryPassSetup();
	void volumePassSetup();
	void lightPassSetup();
	void blitFBO();

	void setModelFP		( glm::mat4 M );
	void setMVPFP		( glm::mat4 MVP );
	void setReverseNor	( bool flag );

	void setModelSP		( glm::mat4 M );
	void setViewSP		( glm::mat4 V );
	void setProjSP		( glm::mat4 P );
	void setLightPos	( glm::vec3 lightPos );

	void setGeomTextures	();
	void setPointLight	( glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse );

};