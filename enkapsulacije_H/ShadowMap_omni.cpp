#include "ShadowMap_omni.h"

bool ShadowMap_omni::Init( int shadow_W, int shadow_H,
		      int window_W, int window_H ){
	bool flag = true;
	SHADOW_WIDTH  = shadow_W;
	SHADOW_HEIGHT = shadow_H;
	WINDOW_WIDTH  = window_W;
	WINDOW_HEIGHT = window_H;

	if( !fillFBO() )
		flag = false;
	if( !loadShaders() )
		flag = false;
return flag;
}

bool ShadowMap_omni::fillFBO(){

	glGenFramebuffers(1, &mFBO);
	glGenTextures(1, &mDepthTexture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthTexture);
	for (GLuint i = 0; i < 6; ++i)
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture, 0);

	if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
	{	std::cerr <<"[CHECK_FB_STATUS]: "<< std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
return true;
}

bool ShadowMap_omni::loadShaders(){
	shader_depthID    = LoadShaders( "shaders/SMomni_depth.vs", "shaders/SMomni_depth.fs", "shaders/SMomni_depth.gs" );
	shader_lightingID = LoadShaders( "shaders/SMomni_light.vs", "shaders/SMomni_light.fs" );
	if( !shader_depthID || !shader_lightingID ) 
		return false;
return true;
}

void ShadowMap_omni::firstPassSetup(){
	glBindFramebuffer ( GL_FRAMEBUFFER, mFBO );
		glViewport( 0, 0, SHADOW_WIDTH, SHADOW_HEIGHT );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
		glUseProgram(shader_depthID);

	glCullFace( GL_FRONT );
	glEnable( GL_CULL_FACE );
}

void ShadowMap_omni::setLightPos( glm::vec3 lightPos ){
   glUniform3fv(glGetUniformLocation(shader_depthID, "lightPos"), 1, &lightPos[0]);
}

void ShadowMap_omni::setModelFP( glm::mat4 Model ){
   glUniformMatrix4fv(glGetUniformLocation(shader_depthID, "model"), 1, GL_FALSE, &Model[0][0]);
}

void ShadowMap_omni::setFarPlaneFP( float far ){
   glUniform1f(glGetUniformLocation(shader_depthID, "far_plane"), far);
}

void ShadowMap_omni::setShadowMatrices( std::vector<glm::mat4> shadowMatrices){
   for (GLuint i = 0; i < 6; ++i)
	glUniformMatrix4fv(glGetUniformLocation(shader_depthID, 
			     ("lightVP[" + std::to_string(i) + "]").c_str()),
			      1, GL_FALSE, &shadowMatrices[i][0][0]);
}

void ShadowMap_omni::secondPassSetup(){
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	glCullFace( GL_BACK );
	glDisable( GL_CULL_FACE );
	glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
		glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
		glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUseProgram(shader_lightingID);
}

void ShadowMap_omni::setModelSP( glm::mat4 Model ){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "model"), 1, GL_FALSE, &Model[0][0]);
}

void ShadowMap_omni::setView(glm::mat4 View){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "view" ), 1, GL_FALSE, &View[0][0]);
}

void ShadowMap_omni::setProj(glm::mat4 Proj){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "projection"), 1, GL_FALSE, &Proj[0][0]);
}

void ShadowMap_omni::setFarPlaneSP( float far ){
   glUniform1f(glGetUniformLocation(shader_lightingID, "far_plane"), far);
}

void ShadowMap_omni::setReverseNor( bool flag ){
   glUniform1f(glGetUniformLocation(shader_lightingID, "reverse_normals"), (int)flag);
}

void ShadowMap_omni::setPointLight( glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse ){
   glUniform3fv(glGetUniformLocation(shader_lightingID, "pLight.position"), 1, &pos[0]);
   glUniform3fv(glGetUniformLocation(shader_lightingID, "pLight.ambient" ), 1, &ambient[0]);
   glUniform3fv(glGetUniformLocation(shader_lightingID, "pLight.diffuse" ), 1, &diffuse[0]);
}

void ShadowMap_omni::setIsPCF( bool isPCF ){
   glUniform1i(glGetUniformLocation(shader_lightingID, "isPCF"), (int)isPCF);
}

void ShadowMap_omni::setDepthTexture( uint offset ){
   glActiveTexture(GL_TEXTURE7);
   glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthTexture);
   glUniform1i(glGetUniformLocation(shader_lightingID, "shadowMap"), 7);
}