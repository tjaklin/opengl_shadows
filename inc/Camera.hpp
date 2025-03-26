#pragma once

#include <vector>

#include "glm-lib/glm.hpp"
#include "glm-lib/gtc/matrix_transform.hpp"

struct Perspective
{
	float fieldOfView;
	float ratio;
	float nearPlane;
	float farPlane;
};

struct Orthogonal
{
    int leftPlane;
    int rightPlane;
    int topPlane;
    int bottomPlane;
    int nearPlane;
    int farPlane;
};

class Camera
{
public:	
    Camera() = default;
    
    void SetViewMatrix(glm::vec3 pos, glm::vec3 dir, glm::vec3 up);

	void SetPerspectiveProjectionMatrix(float fov, float ratio, float near, float far);
	void SetOrthogonalProjectionMatrix(int left, int right, int bottom, int top, int near, int far);
	
    void SetViewAndProjectionMatrix(glm::vec3 pos);

	void HandleMovement(const glm::vec3& movement_direction);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	std::vector<glm::mat4> GetViewAndProjectionMatrix() const;

private:
	glm::vec3 _position;
	glm::vec3 _direction;
	glm::vec3 _up;  // Which way is up.

    // Matrices
	glm::mat4 _view;
	glm::mat4 _projection;
	std::vector<glm::mat4> _view_and_projection;

    // Frustum configurations.
    Perspective _perspective;
    Orthogonal _orthogonal;
};
