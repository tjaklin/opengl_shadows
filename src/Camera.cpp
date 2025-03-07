#include "../inc/Camera.hpp"

void Camera::SetViewMatrix(glm::vec3 pos, glm::vec3 dir, glm::vec3 up)
{
	_view = glm::lookAt( pos, dir, up );

	_position = pos;
	_direction = dir;
	_up = up;
}

void Camera::SetPerspectiveProjectionMatrix(float fov, float ratio, float near, float far)
{
	_perspective.fieldOfView = fov;
	_perspective.ratio = ratio;
	_perspective.nearPlane = near;
	_perspective.farPlane = far;
	
	_projection = glm::perspective(fov, ratio, near, far);
}

void Camera::SetOrthogonalProjectionMatrix(int left, int right, int bottom, int top, int near, int far)
{
	_orthogonal.leftPlane = left;
	_orthogonal.rightPlane = right;
	_orthogonal.topPlane = top;
	_orthogonal.bottomPlane = bottom;
	_orthogonal.nearPlane = near;
	_orthogonal.farPlane = far;

	_projection = glm::ortho<float>(left, right, bottom, top, near, far);
}

void Camera::SetViewAndProjectionMatrix(glm::vec3 pos)
{
	// TODO: Ovo sam samo preimenoval, ali nisam mijenjal logiku. Moram se tu kasnije
	//	vratiti i pregledat dal je potrebno ovo preuredit !
	_view_and_projection.clear();

	_view_and_projection.push_back( _projection * glm::lookAt( pos, pos + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));

	_view_and_projection.push_back( _projection * glm::lookAt( pos, pos + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));

	_view_and_projection.push_back( _projection * glm::lookAt( pos, pos + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)));

	_view_and_projection.push_back( _projection * glm::lookAt( pos, pos + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)));

	_view_and_projection.push_back( _projection * glm::lookAt( pos, pos + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0)));

	_view_and_projection.push_back( _projection * glm::lookAt( pos, pos + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0)));

}

glm::mat4 Camera::GetViewMatrix() const
{
	return _view;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return _projection;
}

std::vector<glm::mat4> Camera::GetViewAndProjectionMatrix() const
{
	return _view_and_projection;
}

// TODO: Ovo ispod je bilo zakomentirano, a samo Bog dragi moze znati
//	kaj bi to trebalo predstavljati. Pregledati to posle i zakljuciti
//	dal mi treba, ili ne !!

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