#include "../inc/Camera.hpp"

void Camera::SetViewMatrix(glm::vec3 pos, glm::vec3 dir, glm::vec3 up)
{
	_view = glm::lookAt(pos, dir, up);

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

// TODO: Currently not in used. Will be used inside of 'Omnidirectional Shadow Map' implementation.
void Camera::SetViewAndProjectionMatrix(glm::vec3 pos)
{
	_view_and_projection.clear();

	_view_and_projection.push_back(_projection * glm::lookAt(pos, pos + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
	_view_and_projection.push_back(_projection * glm::lookAt(pos, pos + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
	_view_and_projection.push_back(_projection * glm::lookAt(pos, pos + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)));
	_view_and_projection.push_back(_projection * glm::lookAt(pos, pos + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)));
	_view_and_projection.push_back(_projection * glm::lookAt(pos, pos + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0)));
	_view_and_projection.push_back(_projection * glm::lookAt(pos, pos + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0)));
}

void Camera::HandleMovement(const glm::vec3& movement_direction)
{
	const float movement_speed = 0.2f;
	_position += movement_direction * movement_speed;

	// Update the View Matrix.
	_view = glm::lookAt(_position, _direction, _up);
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
