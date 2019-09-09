#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera{

private:
	glm::mat4 mView;
	glm::mat4 mProj;
	std::vector<glm::mat4> mViewProj;

	glm::vec3 mPosition;
	glm::vec3 mDirection;
	glm::vec3 mUp;

	float pFoV;
	float pRatio;
	float pNear;
	float pFar;

	int   oLeft;
	int   oRight;
	int   oBottom;
	int   oTop;
	int   oNear;
	int   oFar;

//	bool isOrtho;
//	GLFWwindow* mWindow = 0;
//	bool holdLight = false;
public:
	bool Init();
	
	bool setView( glm::vec3 pos, glm::vec3 dir, glm::vec3 up );
	void setPersProj( float fov, float ratio, float near, float far);
	void setOrthProj( int left, int right, int bottom, int top, int near, int far);
	void setVP( glm::vec3 pos );
	glm::mat4 getView();
	std::vector<glm::mat4> getVP();
	glm::mat4 getProj();
};
#endif