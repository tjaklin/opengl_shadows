#pragma once

#include <GL/glew.h>
#include "glm-lib/glm.hpp"
#include "glm-lib/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL			// Ovo i ovo ispod mi treba ako hocu koristiti
#include "glm-lib/gtx/transform.hpp"	// 'glm::rotate(angle, vector);'. Procitat specku !

class Model
{
public:
	// NOTE: Currently, only a single Vertex Attribute is supported (Position).
	//	It will be necessary to add at least the Normal and TextureUV (Color would be
	//	preferrable too) Vertex Attributes.
	Model(const float* vbo, const int vbo_size);
	~Model();

	void SetScale(float factor);
	void SetRotation(glm::vec3 vector, float angle);
	void SetTranslation(glm::vec3 vector);

	const GLfloat* GetScale();
	const GLfloat* GetRotation();
	const GLfloat* GetTranslation();
	const glm::mat4& GetModelMatrix();

	void Draw(unsigned int attributes) const;

private:
	void RecalculateModelMatrix();

private:
	GLuint _vao = 0;
	GLuint _vbo = 0;
	GLuint _number_of_vertices = 0;
	bool _isVolume = false;

	glm::mat4 _scale  = glm::mat4(1.0f);
	glm::mat4 _rotation = glm::mat4(1.0f);
	glm::mat4 _translation = glm::mat4(1.0f);
	glm::mat4 _model = glm::mat4(1.0f);
};
