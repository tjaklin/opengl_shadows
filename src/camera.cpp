#include "camera.h"

bool Camera::setView( glm::vec3 pos, glm::vec3 dir, glm::vec3 up ){

	mPosition	= pos;
	mDirection	= dir;
	mUp		= up;
	
	mView		= glm::lookAt( pos, dir, up );
}

void Camera::setPersProj( float fov, float ratio, float near, float far){
	pFoV 	= fov;
	pRatio	= ratio;
	pNear	= near;
	pFar	= far;

	mProj	= glm::perspective( pFoV, pRatio, pNear, pFar );
}

void Camera::setOrthProj( int left, int right, int bottom, int top, int near, int far){
	oLeft	= left;
	oRight	= right;
	oBottom	= bottom;
	oTop	= top;
	oNear	= near;
	oFar	= far;

	mProj	= glm::ortho<float>( oLeft, oRight, oBottom, oTop, oNear, oFar);
}

void Camera::setVP( glm::vec3 pos ){
	mViewProj.clear();

mViewProj.push_back( mProj * 
	glm::lookAt( pos, pos + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
mViewProj.push_back( mProj * 
	glm::lookAt( pos, pos + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
mViewProj.push_back( mProj * 
	glm::lookAt( pos, pos + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)));
mViewProj.push_back( mProj * 
	glm::lookAt( pos, pos + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)));
mViewProj.push_back( mProj * 
	glm::lookAt( pos, pos + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0)));
mViewProj.push_back( mProj * 
	glm::lookAt( pos, pos + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0)));
}

glm::mat4 Camera::getView(){
	return mView;
}

std::vector<glm::mat4> Camera::getVP(){
	return mViewProj;
}

glm::mat4 Camera::getProj(){
	return mProj;
}

//bool Camera::getHoldLight(){ return holdLight;}

/*void Camera::pollKeyboard( mWindow ){

	if (glfwGetKey( mWindow, GLFW_KEY_W ) == GLFW_PRESS){
//		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( mWindow, GLFW_KEY_S ) == GLFW_PRESS){
//		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( mWindow, GLFW_KEY_D ) == GLFW_PRESS){
//		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( mWindow, GLFW_KEY_A ) == GLFW_PRESS){
//		position -= right * deltaTime * speed;
	}
	if (glfwGetKey( mWindow, GLFW_KEY_L ) == GLFW_PRESS){
		if (glfwGetKey( mWindow, GLFW_KEY_L ) == GLFW_RELEASE)
			holdLight = !holdLight;
	}
}
*/