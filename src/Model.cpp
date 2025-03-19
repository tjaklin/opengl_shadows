#include "../inc/Model.hpp"

#include <cstdio>

Model::Model()
{
	glGenVertexArrays(1, &_vao);
}

Model::~Model()
{
	glDeleteVertexArrays(1, &_vao);
	_vao = 255;	// 255 means invalid value.

	// If _vbo hodls a valid buffer in OpenGL land, delete it!
	if (_vbo != 255)
	{
		glDeleteBuffers(1, &_vbo);
		_vbo = 255; // 255 means invalid value.
	}
}

void Model::PushVertexAttribute(VertexAttribute& attribute, unsigned int location)
{
	// TODO: This method could still use some cleaning up !

	// If _vbo already holds a buffer in OpenGL, make sure to
	//	copy it's data to the new buffer that we're about to
	//	create.
	if (_vbo == 255)
	{
		GLuint new_vbo = 255;
		glGenBuffers(1, &new_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, new_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attribute.count,
			attribute.data.data(), GL_STATIC_DRAW);

		glBindVertexArray(_vao);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (const void*) 0);// Position only.
		_vbo = new_vbo;
	}
	else
	{
		// TODO: What will 'current_size' be if _vbo is '255' ?
		GLint current_size = 0;
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &current_size);

		GLint new_size = current_size + (sizeof(float) * attribute.count);
		GLuint new_vbo;
		glGenBuffers(1, &new_vbo);
		glBindBuffer(GL_COPY_WRITE_BUFFER, new_vbo);
		glBufferData(GL_COPY_WRITE_BUFFER, new_size, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_COPY_READ_BUFFER, _vbo);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, current_size);
		glBufferSubData(GL_COPY_WRITE_BUFFER, current_size,
			sizeof(float) * attribute.count, attribute.data.data());

		// Unbind the Read and Write targets.
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		// Delete the old one and keep the new vbo handle.
		glDeleteBuffers(1, &_vbo);
		_vbo = new_vbo;

		// Bind the Array target and VAO.
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// TODO: Correctly set up the VertexAttribute pointers.
		//	Currently it works only because i know what VAttributes i want to use
		//	look like . . . Make it better later !
		(void) location;	// Use this instead, once the time comes.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (const void*) 0);			// Position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (const void*) current_size);	// Color
	}

	// Unbind VAO after setting all the data.
	glBindVertexArray(0);

	_number_of_vertices = (GLuint) attribute.count / 3;	// Divide by 3 dimensions.
}

void Model::Draw(unsigned int attributes) const
{
	// Currently, only a single Vertex Attribute is supported and that is Position.
	glBindVertexArray(_vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, _number_of_vertices);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);

	(void) attributes;
	return; // Return right after drawing is done.

	// TODO: Check and enable this later
/*
	switch(attributes)
	{
	case 3:
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*) (6*sizeof(GLfloat)) );

	default:
	case 2:
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*) (3*sizeof(GLfloat)) );

	case 1:
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0 );
	};

	glDrawArrays(GL_TRIANGLES, 0, _number_of_vertices );
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
*/
}

void Model::RecalculateModelMatrix()
{
	_model = _translation * _rotation * _scale;
}

void Model::SetScale(float factor)
{
	_scale = glm::scale(glm::mat4(1.0f), glm::vec3(factor));
	RecalculateModelMatrix();

}
void Model::SetRotation(glm::vec3 vector, float angle)
{
	_rotation = glm::rotate(angle, vector);	// Za ovo treba 'GLM EXPERIMENTAL'
	RecalculateModelMatrix();

}
void Model::SetTranslation(glm::vec3 vector)
{
	_translation = glm::translate(glm::mat4(1.0f), vector);
	RecalculateModelMatrix();
}

const GLfloat* Model::GetScale() { return &_scale[0][0]; }
const GLfloat* Model::GetRotation() { return &_rotation[0][0]; }
const GLfloat* Model::GetTranslation() { return &_translation[0][0]; }
const glm::mat4& Model::GetModelMatrix() { return _model; }
