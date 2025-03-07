#include "ShadowMap_variance.h"

bool ShadowMap_variance::Init( int window_W, int window_H ){
	bool flag = true;
	WINDOW_WIDTH  = window_W;
	WINDOW_HEIGHT = window_H;

	if( !fillFBO_depth() )
		flag = false;
	if( !fillFBO_blur() )
		flag = false;
	if( !loadShaders() )
		flag = false;
return flag;
}

bool ShadowMap_variance::fillFBO_blur(){
	const float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	glGenTextures(1, &mBlurTexture);
	glGenFramebuffers(1, &mFBO_blur);

	glBindTexture(GL_TEXTURE_2D, mBlurTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, WINDOW_WIDTH * SHADOW_COEF * BLUR_COEF, WINDOW_HEIGHT * SHADOW_COEF * BLUR_COEF, 0, GL_RGBA, GL_FLOAT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO_blur);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurTexture, 0);

	if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE ){
		std::cerr <<"[CHECK_FB_STATUS]: "<< std::endl;
		return false;
		}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

return true;
}

bool ShadowMap_variance::fillFBO_depth(){
	const float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	glGenTextures(1, &mDepthTexture);
	glGenFramebuffers(1, &mFBO_depth);

	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, WINDOW_WIDTH * SHADOW_COEF, WINDOW_HEIGHT * SHADOW_COEF, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO_depth);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mDepthTexture, 0);

	if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE ){
		std::cerr <<"[CHECK_FB_STATUS]: "<< std::endl;
		return false;
		}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

return true;
}

bool ShadowMap_variance::loadShaders(){
	shader_depthID    = LoadShaders( "shaders/SMvar_depth.vs", "shaders/SMvar_depth.fs" );
	shader_blurID    = LoadShaders( "shaders/SMvar_blur.vs", "shaders/SMvar_blur.fs" );
	shader_lightingID = LoadShaders( "shaders/SMvar_light.vs", "shaders/SMvar_light.fs" );
	if( !shader_depthID || !shader_lightingID || !shader_blurID ) 
		return false;
return true;
}

void ShadowMap_variance::depthPassSetup(){
	glColorMask(GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO_depth);
	glDrawBuffer( GL_COLOR_ATTACHMENT0 );
		glViewport(0,0, WINDOW_WIDTH * SHADOW_COEF, WINDOW_HEIGHT * SHADOW_COEF);
		glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);

	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
		glUseProgram(shader_depthID);
}

void ShadowMap_variance::setLightMVP( glm::mat4 lMVP ){
   glUniformMatrix4fv(glGetUniformLocation(shader_depthID, "lMVP"), 1, GL_FALSE, &lMVP[0][0]);
}

void ShadowMap_variance::blurXPassSetup(){
	glDisable( GL_CULL_FACE );
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO_blur);
	glViewport(0, 0, WINDOW_WIDTH * SHADOW_COEF * BLUR_COEF, WINDOW_HEIGHT * SHADOW_COEF * BLUR_COEF);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE);

	glUseProgram(shader_blurID);
}

void ShadowMap_variance::blurYPassSetup(){
	glDisable( GL_CULL_FACE );
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO_depth);
		glColorMask( GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE );
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, WINDOW_WIDTH * SHADOW_COEF, WINDOW_HEIGHT * SHADOW_COEF);

	glUseProgram(shader_blurID);
}

void ShadowMap_variance::setDepthTextureB( bool isXpass ){
   glActiveTexture( GL_TEXTURE3 );
	if( isXpass )
	   glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	else
	   glBindTexture(GL_TEXTURE_2D, mBlurTexture);
   glUniform1i(glGetUniformLocation(shader_blurID, "shadowMap"), 3);
}

void ShadowMap_variance::setFilterValX(){
   glUniform2f( glGetUniformLocation(shader_blurID,"ScaleU") , 1.0/(WINDOW_WIDTH * 0.5 * 0.25), 0.0);
}

void ShadowMap_variance::setFilterValY(){
   glUniform2f( glGetUniformLocation(shader_blurID,"ScaleU"), 0.0, 1.0/(WINDOW_HEIGHT * 0.5));
}

void ShadowMap_variance::lightPassSetup(){
	glDisable( GL_CULL_FACE );

	glBindFramebuffer(GL_FRAMEBUFFER,0);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_lightingID);
}

void ShadowMap_variance::setLightDir( glm::vec3 lightDir){
   glUniform3f(glGetUniformLocation(shader_lightingID, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
}

void ShadowMap_variance::setModel(glm::mat4 Model){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "model"), 1, GL_FALSE, &Model[0][0]);
}

void ShadowMap_variance::setView(glm::mat4 View){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "view" ), 1, GL_FALSE, &View[0][0]);
}

void ShadowMap_variance::setProj(glm::mat4 Proj){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "projection"), 1, GL_FALSE, &Proj[0][0]);
}

void ShadowMap_variance::setBiasedLightMVP( glm::mat4 biasedlMVP ){
   glUniformMatrix4fv(glGetUniformLocation(shader_lightingID, "lBiasMVP"), 1, GL_FALSE, &biasedlMVP[0][0]);
}

void ShadowMap_variance::setDepthTextureL(){
   glActiveTexture( GL_TEXTURE3 );
   glBindTexture(GL_TEXTURE_2D, mDepthTexture);
   glUniform1i(glGetUniformLocation(shader_lightingID, "shadowMap"), 3);
}