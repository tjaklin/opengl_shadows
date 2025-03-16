#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "loadShader.h"

class ShadowMap_dir{

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

	void setLightMVP( glm::mat4 lMVP);

	void setLightDir( glm::vec3 lightDir);
	void setModel	( glm::mat4 Model);
	void setView	( glm::mat4 View );
	void setProj	( glm::mat4 Proj );
	void setBiasedLightMVP( glm::mat4 biasedlMVP);
	void setIsPCF	( bool isPCF );
	void setDepthTexture( uint offset = 1);
};