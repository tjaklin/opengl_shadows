#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "meshVertices.cpp"

class Model{

private:
	GLuint mVBO = 0;
	GLuint mEBO = 0;
	GLuint mNUM_VERTICES = 0;
	bool isVolume = false;

	glm::mat4 mScale  = glm::mat4(1.0f);
	glm::mat4 mRotate = glm::mat4(1.0f);
	glm::mat4 mTranslate = glm::mat4(1.0f);
	glm::mat4 mModel = glm::mat4(1.0f);

	void drawArray( uint attributes );
	void drawElements();
public:
	Model();

	void setScale( float factor );
	void setRotate( glm::vec3 vector, float angle );
	void setTranslate( glm::vec3 vector );
	void setModel();

	const GLfloat* getScale();
	const GLfloat* getRotate();
	const GLfloat* getTranslate();
	const GLfloat* getModelFLOAT();
	glm::mat4      getModelMAT();

	void drawModel( uint attributes = 0 );
	bool fillData(const GLfloat* vbo, const int vbo_size);
	bool fillData(const GLfloat* vbo, const int vbo_size, 
			const GLuint* ebo, const int ebo_size);
};