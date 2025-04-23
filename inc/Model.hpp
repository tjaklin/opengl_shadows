#pragma once

#include "GL/glew.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL			// Ovo i ovo ispod mi treba ako hocu koristiti
#include "gtx/transform.hpp"			// 'glm::rotate(angle, vector);'. Procitat specku !

#include "VertexAttributeParser.hpp"

class Model
{
public:
	Model();
	~Model();

	void SetScale(const glm::vec3& scaleVector);
	void SetRotation(glm::vec3 vector, float angle);
	void SetTranslation(glm::vec3 vector);

	const GLfloat* GetScale();
	const GLfloat* GetRotation();
	const GLfloat* GetTranslation();
	const glm::mat4& GetModelMatrix();

	void Draw() const;
	void DrawVolume() const;
	void PushVertexAttribute(VertexAttribute& attribute, unsigned int location);
	void SetElementArrayBuffer(VertexAttribute& attribute);
	
private:
	void RecalculateModelMatrix();

private:
	GLuint _vao = 255;	// 255 means invalid value.
	GLuint _vbo = 255;	// 255 means invalid value.
	GLuint _ebo = 255;	// 255 means invalid value.
	GLuint _number_of_vertices = 0;

	glm::mat4 _scale  = glm::mat4(1.0f);
	glm::mat4 _rotation = glm::mat4(1.0f);
	glm::mat4 _translation = glm::mat4(1.0f);
	glm::mat4 _model = glm::mat4(1.0f);

	std::vector<VertexAttributeLayout> _attribute_layouts;
};
