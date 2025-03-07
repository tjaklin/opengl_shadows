#include "ShadowVolume.h"

bool ShadowVolume::Init( int shadow_W, int shadow_H,
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

bool ShadowVolume::fillFBO(){
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	// Position
	glGenTextures(1, &mPosTex);
	glBindTexture(GL_TEXTURE_2D, mPosTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPosTex, 0);

	// Normal
	glGenTextures(1, &mNormTex);
	glBindTexture(GL_TEXTURE_2D, mNormTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormTex, 0);

	// Color
	glGenTextures(1, &mColorTex);
	glBindTexture(GL_TEXTURE_2D, mColorTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mColorTex, 0);

	// Depth
	glGenTextures(1, &mDepthTex);
	glBindTexture(GL_TEXTURE_2D, mDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTex, 0);

	// Final Texture
	glGenTextures(1, &mFinalTex);
	glBindTexture(GL_TEXTURE_2D, mFinalTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, mFinalTex, 0);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if( Status != GL_FRAMEBUFFER_COMPLETE ){
		std::cerr <<"[CHECK_FB_STATUS]: "<<Status<< std::endl;
		return false;}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

return true;
}

bool ShadowVolume::loadShaders(){
	shader_geometryID = LoadShaders( "shaders/SV_geom.vs", "shaders/SV_geom.fs" );
	shader_volumeID   = LoadShaders( "shaders/SV_volume.vs", "shaders/SV_volume.fs", "shaders/SV_volume.gs" );
	shader_lightingID = LoadShaders( "shaders/SV_light.vs", "shaders/SV_light.fs" );
	if( !shader_geometryID || !shader_lightingID || !shader_volumeID ) 
		return false;
return true;
}

void ShadowVolume::geometryPassSetup(){
//			0th pass		//
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
//			1st pass		//

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
	GLenum foo[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers( 3, foo );
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glUseProgram( shader_geometryID );
	glDisable(GL_CULL_FACE );
	glDepthMask(GL_TRUE);
}

void ShadowVolume::setModelFP( glm::mat4 M ){
    glUniformMatrix4fv(glGetUniformLocation( shader_geometryID, "model" ),
			   1, GL_FALSE, &M[0][0]);
}

void ShadowVolume::setMVPFP( glm::mat4 MVP ){
    glUniformMatrix4fv(glGetUniformLocation( shader_geometryID, "MVP" ),
			   1, GL_FALSE, &MVP[0][0]);
}

void ShadowVolume::setReverseNor( bool flag ){
    glUniform1i( glGetUniformLocation( shader_geometryID, "reverse_normals"),(int) flag);
}

void ShadowVolume::volumePassSetup(){
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_CLAMP);
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	glDisable(GL_CULL_FACE );

	glUseProgram( shader_volumeID );
	glEnable(GL_STENCIL_TEST);
	glClear ( GL_STENCIL_BUFFER );

	glStencilOpSeparate( GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP );
	glStencilFunc( GL_ALWAYS, 0, 0xff );
		glStencilMask( 0xff );
}

void ShadowVolume::setModelSP( glm::mat4 M ){
    glUniformMatrix4fv(glGetUniformLocation( shader_volumeID, "model" ),
			   1, GL_FALSE, &M[0][0] );
}

void ShadowVolume::setViewSP( glm::mat4 V ){
    glUniformMatrix4fv(glGetUniformLocation( shader_volumeID, "view" ),
			   1, GL_FALSE, &V[0][0] );
}

void ShadowVolume::setProjSP( glm::mat4 P ){
    glUniformMatrix4fv(glGetUniformLocation( shader_volumeID, "projection" ),
			   1, GL_FALSE, &P[0][0] );
}

void ShadowVolume::setLightPos( glm::vec3 lightPos ){
    glUniform3f(glGetUniformLocation( shader_volumeID, "gLightPos" ), 
		lightPos.x, lightPos.y, lightPos.z);
}

void ShadowVolume::lightPassSetup(){
	glDisable(GL_DEPTH_CLAMP);
	glDrawBuffer( GL_COLOR_ATTACHMENT4 );
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	glUseProgram( shader_lightingID );

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mPosTex);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mNormTex);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, mColorTex);

	glDepthMask(GL_TRUE);
}

void ShadowVolume::setGeomTextures(){
    glUniform1i( glGetUniformLocation( shader_lightingID, "mPos"), 2);
    glUniform1i( glGetUniformLocation( shader_lightingID, "mNor"), 3);
    glUniform1i( glGetUniformLocation( shader_lightingID, "mCol"), 4);
}

void ShadowVolume::setPointLight( glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse ){
    glUniform3f( glGetUniformLocation( shader_lightingID, "pLight.position"), pos.x, pos.y, pos.z);
    glUniform3f( glGetUniformLocation( shader_lightingID, "pLight.ambient"), ambient.x, ambient.y, ambient.z);
    glUniform3f( glGetUniformLocation( shader_lightingID, "pLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
}

void ShadowVolume::blitFBO(){
	
	glDisable( GL_STENCIL_TEST );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, mFBO );
	glReadBuffer( GL_COLOR_ATTACHMENT4 );

	glBlitFramebuffer( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			   0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			   GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glStencilMask( 0xff );
}