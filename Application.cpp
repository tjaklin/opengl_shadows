#include "glm-lib/glm.hpp"  // Ignore this. VScode can't find it, but G++ can via -I from Makefile.
// TODO: Ubacit sve ove '3rd party dependencije' u Dependency folder, koji je dio projekta samoga !

#include <cstdio>

#include "inc/Window.hpp"
/*
    #include "inc/Model.h"
    #include "inc/Camera.h"
    #include "inc/ShadowMap_dir.h"
    #include "inc/ShadowMap_omni.h"
    #include "inc/ShadowVolume.h"
    #include "inc/ShadowMap_variance.h"
*/

/*
void checKeyState( uint* state, bool* isMoving, bool* isPCF, GLFWwindow* mWindow ){
	if (glfwGetKey( mWindow, GLFW_KEY_1 ) == GLFW_PRESS)
		*state = 1;
	if (glfwGetKey( mWindow, GLFW_KEY_2 ) == GLFW_PRESS)
		*state = 2;
	if (glfwGetKey( mWindow, GLFW_KEY_3 ) == GLFW_PRESS)
		*state = 3;
	if (glfwGetKey( mWindow, GLFW_KEY_4 ) == GLFW_PRESS)
		*state = 4;
	if (glfwGetKey( mWindow, GLFW_KEY_L ) == GLFW_PRESS)
		if (glfwGetKey( mWindow, GLFW_KEY_L ) == GLFW_RELEASE)
			*isMoving = !*isMoving;
	if (glfwGetKey( mWindow, GLFW_KEY_Q ) == GLFW_PRESS)
		if (glfwGetKey( mWindow, GLFW_KEY_Q ) == GLFW_RELEASE)
			*isPCF = !*isPCF;
}
*/

int main(int argc, char** argv)
{
    // Ignore input data for now.
    (void) argc; (void) argv;

    Window window(640, 480, "OpenGL Sjene");

    while (!window.ShouldClose())
    {
        window.Update();
        window.Draw();
    }

    return 0;
}


/*

int old_main_version()
{
const GLint WINDOW_WIDTH = 1024,  WINDOW_HEIGHT = 1024,
	  SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const GLfloat omni_PROJ_RATIO = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;

	GLFWwindow* mWindow = initEverything(WINDOW_WIDTH, WINDOW_HEIGHT);
		if(!mWindow)	return -1;

glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(mWindow, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearStencil(0);

	glClearColor(0.75f, 0.52f, 0.3f, 1.0f);

//////////////////////////////////////////
	GLuint VAOID;			//
	glGenVertexArrays(1, &VAOID);	//
	glBindVertexArray(VAOID);	//
//////////////////////////////////////////
// DATA DEFINITION //
/////////////////////
Camera eye, dir_bulb, omni_bulb;
	struct light {
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
	} pLight, dLight;

	pLight.position = glm::vec3(-3.0f, 3.3f,-1.5f);
	pLight.ambient  = glm::vec3(0.2f);
	pLight.diffuse  = glm::vec3(0.8f);

	dLight.position = glm::vec3(-4.0, 6.0f,-2.0f);
	dLight.ambient  = glm::vec3(0.2f);
	dLight.ambient	= glm::vec3(0.8f);

glm::vec3 cubePositions[] = {
	glm::vec3(-4.0f, 2.5f, 7.2f ),
	glm::vec3( 8.0f,-0.7f, 8.3f ),
	glm::vec3( 3.0f, 4.0f, 3.0f ),
	glm::vec3(-2.0f,-1.7f,-2.0f ),
	glm::vec3(-3.0f, 1.5f,-1.0f ),
	};

glm::vec3 camPositions[] = { 
	glm::vec3(-5.0f, 2.0f,-5.0f),
	glm::vec3(-8.5f,-3.9f,-6.5f),
	glm::vec3( 0.0f,12.0f,-10.0f),
	glm::vec3( 2.5f,-3.0f,-3.0f), 
	};

eye.setView( camPositions[0], glm::vec3( 3.0f, -2.7f, 3.0f), glm::vec3(0,1,0) );
eye.setPersProj( 45.0f, 4.0f/3.0f, 1.0f, 100.0f );

dir_bulb.setView( dLight.position, cubePositions[4], glm::vec3(0,1,0) );
dir_bulb.setOrthProj(-10, 10,-20, 20,-10, 20);

omni_bulb.setPersProj( glm::radians(90.0f), omni_PROJ_RATIO, 1.0f, 100.0f );
omni_bulb.setVP( pLight.position );

glm::mat4 lightMVP = glm::mat4(1.0f);
glm::mat4 lightBiasMVP = glm::mat4(1.0f);
glm::mat4 biasMVP = glm::mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);

Model kocka, podloga, platno, volumen;

std::vector <Model> kocke;
	for(uint i=0; i<4; i++){
		kocke.push_back(Model());
		if( !kocke[i].fillData( &shapeCube_PosNorm[0], sizeof(shapeCube_PosNorm)) )
			return false;
	}
	if( !kocka.fillData( &shapeCube_PosNorm[0], sizeof(shapeCube_PosNorm)) )
	return false;

	if( !podloga.fillData( &shapePlaneVertices[0], sizeof(shapePlaneVertices)) )
	return false;

	if( !platno.fillData( &shapeQuadVertices[0], sizeof(shapeQuadVertices)) )
	return false;

	if( !volumen.fillData( &shapeCubeVertices[0], sizeof(shapeCubeVertices),
	&shapeCubeIndices[0], sizeof(shapeCubeIndices)) )
	return false;

//kocka.loadTexture	("uvtemplate.bmp");
//podloga.loadTexture	("uvtemplate.bmp");

kocka.setScale		( 15.0f );
kocka.setTranslate	( glm::vec3(6.0f, 2.3f, 4.6f ) );
podloga.setTranslate	( glm::vec3(0.0f, 0.0f, 0.0f ) );

for(uint i=0; i<4; i++){
	kocke[i].setTranslate( cubePositions[i] );
	kocke[i].setModel();
}

kocka.setModel();
podloga.setModel();

ShadowMap_dir SM_dir;
ShadowMap_omni SM_point;
ShadowVolume SV;
ShadowMap_variance SM_var;

SM_dir.Init	( 1024, 1024, 1024, 1024 );
SM_var.Init	( 1024, 1024 );
SM_point.Init	( 1024, 1024, 1024, 1024 );
SV.Init		( 1024, 1024, 1024, 1024 );

float pomak = 0.08f;
char dFlag = 1;
char pFlag= 1;
bool isMoving = true;
bool isPCF = false;
bool isDir = false;

uint state = 1;

do{
////////////////////////////////////////////////////////////////////////////////////////////
// LIGHT SETUP
	if( isMoving ){
	if( ((pLight.position.x > 0) && (pLight.position.x > 4.0f) )
	      || ((pLight.position.x < 0) && (pLight.position.x < -4.0f)) )
		pFlag *= -1;
	pLight.position.x += pomak * pFlag;
	omni_bulb.setVP( pLight.position );

	if( ((dLight.position.x > 0) && (dLight.position.x > 5.0f) )
	      || ((dLight.position.x < 0) && (dLight.position.x < -5.0f)) )
		dFlag *= -1;
	dLight.position.x += pomak * dFlag;
	dir_bulb.setView( dLight.position, cubePositions[4], glm::vec3(0,1,0) );
	}
////////////////////////////////////////////////////////////////////////////////////////////
	kocke[0].setTranslate( cubePositions[0] );
	kocke[0].setModel();

if( state == 1 ){
	
	dir_bulb.setOrthProj(-10, 10,-20, 20,-10, 20);
	dir_bulb.setView( dLight.position, cubePositions[4], glm::vec3(0,1,0) );
	eye.setView( camPositions[0] , cubePositions[4], glm::vec3(0,1,0));
	kocke[0].setTranslate( cubePositions[4] );
	kocke[0].setModel();

	// DIRECTIONAL SHADOW MAP
	SM_dir.firstPassSetup();
		lightMVP = dir_bulb.getProj() * dir_bulb.getView() * kocke[0].getModelMAT();
		SM_dir.setLightMVP( lightMVP );
			kocke[0].drawModel(1);

	SM_dir.secondPassSetup();
		SM_dir.setLightDir( dLight.position );
		SM_dir.setModel( kocke[0].getModelMAT() );
		SM_dir.setView	( eye.getView() );
		SM_dir.setProj	( eye.getProj() );
		lightBiasMVP = biasMVP * lightMVP;
		SM_dir.setBiasedLightMVP( lightBiasMVP );
		SM_dir.setIsPCF	( isPCF );
		SM_dir.setDepthTexture( 1 );
			kocke[0].drawModel();

		SM_dir.setModel( podloga.getModelMAT() );
		lightMVP = dir_bulb.getProj() * dir_bulb.getView() * podloga.getModelMAT();
		lightBiasMVP = biasMVP * lightMVP;
		SM_dir.setBiasedLightMVP( lightBiasMVP );
			podloga.drawModel();

}

if( state == 2 ){

	dir_bulb.setView( dLight.position, cubePositions[4], glm::vec3(0,1,0) );
	dir_bulb.setOrthProj(-10, 10,-10, 10,-10, 40);
	eye.setView( camPositions[0], cubePositions[4], glm::vec3(0,1,0));
	kocke[0].setTranslate( cubePositions[4] );
	kocke[0].setModel();

	// DIRECTIONAL SHADOW MAP
	SM_var.depthPassSetup();
		lightMVP = dir_bulb.getProj() * dir_bulb.getView() * kocke[0].getModelMAT();
		SM_dir.setLightMVP( lightMVP );
			kocke[0].drawModel(1);

	SM_var.blurXPassSetup();
		SM_var.setFilterValX();
		SM_var.setDepthTextureB( true );
			platno.drawModel(3);

	SM_var.blurYPassSetup();
		SM_var.setFilterValY();
		SM_var.setDepthTextureB( false );
			platno.drawModel(3);

	SM_var.lightPassSetup();
		SM_var.setLightDir( dLight.position );
		SM_var.setView	( eye.getView() );
		SM_var.setProj	( eye.getProj() );
		SM_var.setDepthTextureL();

		SM_var.setModel	( kocke[0].getModelMAT() );
			lightMVP = dir_bulb.getProj() * dir_bulb.getView() * kocke[0].getModelMAT();
			lightBiasMVP = biasMVP * lightMVP;
		SM_var.setBiasedLightMVP( lightBiasMVP );
			kocke[0].drawModel();

		SM_var.setModel	( podloga.getModelMAT() );
			lightMVP = dir_bulb.getProj() * dir_bulb.getView() * podloga.getModelMAT();
			lightBiasMVP = biasMVP * lightMVP;
		SM_var.setBiasedLightMVP( lightBiasMVP );
			podloga.drawModel();


}

if( state == 3){
	eye.setView( camPositions[1] , cubePositions[0]+glm::vec3(0,-6.4,0), glm::vec3(0,1,0));

	// POINT LIGHT SHADOW MAP
	SM_point.firstPassSetup();
	for(uint i=0; i<4; i++){
		SM_point.setLightPos	 ( pLight.position );
		SM_point.setModelFP	 ( kocke[i].getModelMAT() );
		SM_point.setFarPlaneFP	 ( 100.0f );
		SM_point.setShadowMatrices( omni_bulb.getVP() );
			kocke[i].drawModel();
	}

	SM_point.secondPassSetup();
		SM_point.setView	( eye.getView() );
		SM_point.setProj	( eye.getProj() );
		SM_point.setFarPlaneSP	( 100.0f );
		SM_point.setReverseNor	( false );
		SM_point.setPointLight	( pLight.position, pLight.ambient, pLight.diffuse );
		SM_point.setIsPCF	( isPCF );
		SM_point.setDepthTexture(2);
	for(uint i=0; i<4; i++){
		SM_point.setModelSP	( kocke[i].getModelMAT());
			kocke[i].drawModel();
	}
		SM_point.setReverseNor	( true );
		SM_point.setModelSP( kocka.getModelMAT() );
			kocka.drawModel();
}

if( state == 4 ){
	eye.setView( camPositions[2], cubePositions[2], glm::vec3(0,1,0) );
	// SHADOW VOLUME
	SV.geometryPassSetup();
		SV.setModelFP( kocka.getModelMAT() );
		SV.setMVPFP( eye.getProj() * eye.getView() * kocka.getModelMAT());
		SV.setReverseNor( true );
			kocka.drawModel();

		SV.setReverseNor( false );
		for(uint i=0; i<4; i++){
		SV.setModelFP( kocke[i].getModelMAT() );
		SV.setMVPFP( eye.getProj() * eye.getView() * kocke[i].getModelMAT());
			kocke[i].drawModel();
		}

	SV.volumePassSetup();
		SV.setViewSP( eye.getView() );
		SV.setProjSP( eye.getProj() );
		SV.setLightPos( pLight.position );
	for(uint i=0; i<4; i++){
		SV.setModelSP( kocke[i].getModelMAT() );
			volumen.drawModel();
	}

	SV.lightPassSetup();
		SV.setGeomTextures();
		SV.setPointLight( pLight.position, pLight.ambient, pLight.diffuse );

		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		glStencilFunc( GL_EQUAL, 0, 0xff );
		glStencilMask( 0x00 );
			platno.drawModel(1);

		SV.setGeomTextures();
		SV.setPointLight( pLight.position, pLight.ambient, glm::vec3(0.0f) );

		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		glStencilFunc( GL_NOTEQUAL, 0, 0xff );
		glStencilMask( 0x00 );
			platno.drawModel(1);

	SV.blitFBO();
}

	glUseProgram( 0 );

// Swap buffers
glfwSwapBuffers( mWindow );
glfwPollEvents();
checKeyState( &state, &isMoving, &isPCF, mWindow );

}while(glfwGetKey(mWindow, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(mWindow) == 0);

// CLEAN UP
glDeleteVertexArrays(1, &VAOID);

glfwTerminate();

return 0;
}

*/
