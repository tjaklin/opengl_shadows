#include "ShadowMap_dir.h"

bool ShadowMap_dir::Init( int shadow_W, int shadow_H,
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

bool ShadowMap_dir::fillFBO(){
	glGenFramebuffers(1, &mFBO);

	glGenTextures(1, &mDepthTexture);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture, 0);

	if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
	{	std::cerr <<"[CHECK_FB_STATUS]: "<< std::endl;
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
return true;
}



bool ShadowMap_dir::loadShaders(){
	shader_depthID    = LoadShaders( "shaders/SMdir_depth.vs", "shaders/SMdir_depth.fs" );
	shader_lightingID = LoadShaders( "shaders/SMdir_light.vs", "shaders/SMdir_light.fs" );
	if( !shader_depthID || !shader_lightingID ) 
		return false;
return true;
}

void ShadowMap_dir::firstPassSetup(){
	glBindFramebuffer ( GL_FRAMEBUFFER, mFBO );
		glViewport( 0, 0, SHADOW_WIDTH, SHADOW_HEIGHT );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
		glUseProgram(shader_depthID);

	glCullFace( GL_FRONT );
	glEnable( GL_CULL_FACE );
}

void ShadowMap_dir::setLightMVP( glm::mat4 lMVP ){
   glUniformMatrix4fv(glGetUniformLocation(shader_depthID, "lMVP"), 1, GL_FALSE, &lMVP[0][0]);
}

void ShadowMap_dir::secondPassSetup(){
	glCullFace( GL_BACK );
	glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
		glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
		glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glUseProgram(shader_lightingID);
}

void ShadowMap_dir::setLightDir( glm::vec3 lightDir){
   glUniform3f(glGetUniformLocation(shader_lightingID, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
}

void ShadowMap_dir::setModel(glm::mat4 Model){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "model"), 1, GL_FALSE, &Model[0][0]);
}

void ShadowMap_dir::setView(glm::mat4 View){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "view" ), 1, GL_FALSE, &View[0][0]);
}

void ShadowMap_dir::setProj(glm::mat4 Proj){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "projection"), 1, GL_FALSE, &Proj[0][0]);
}

void ShadowMap_dir::setBiasedLightMVP( glm::mat4 biasedlMVP ){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "lBiasMVP"), 1, GL_FALSE, &biasedlMVP[0][0]);
}

void ShadowMap_dir::setIsPCF( bool isPCF ){
   glUniform1i(glGetUniformLocation(shader_lightingID, "isPCF"), (int)isPCF);
}

void ShadowMap_dir::setDepthTexture( uint offset ){
   glActiveTexture(GL_TEXTURE0 + offset);
   glBindTexture(GL_TEXTURE_2D, mDepthTexture);
   glUniform1i(glGetUniformLocation(shader_lightingID, "shadowMap"), offset);
}