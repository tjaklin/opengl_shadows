#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <cstring>
#include <string>
#include <vector>

#include "loadShader.h"

class ShadowMap_omni{

private:
	int SHADOW_WIDTH  = 0;
	int SHADOW_HEIGHT = 0;
	int WINDOW_WIDTH  = 0;
	int WINDOW_HEIGHT = 0;

	GLuint mFBO;
	GLuint mDepthTexture;

	GLuint shader_depthID;
	GLuint shader_lightingID;

	bool loadShaders();
	bool fillFBO();

public:
	bool Init( int shadow_W, int shadow_H,
		   int window_W, int window_H );
	void firstPassSetup();
	void secondPassSetup();

	void setLightPos( glm::vec3 lightPos);
	void setModelFP	( glm::mat4 Model);
	void setFarPlaneFP( float far );
	void setShadowMatrices( std::vector<glm::mat4> shadowMatrices);

	void setModelSP		( glm::mat4 Model);
	void setView		( glm::mat4 View );
	void setProj		( glm::mat4 Proj );
	void setFarPlaneSP	( float far );
	void setReverseNor	( bool areReversed );
	void setPointLight	( glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse );
	void setIsPCF		( bool isPCF );
	void setDepthTexture	( uint offset = 1 );
};