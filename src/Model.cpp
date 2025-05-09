#include "Model.hpp"

#include <cstdio>

Model::Model()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_ebo);
}

Model::~Model()
{
	// If _vbo holds a valid buffer in OpenGL land, delete it!
	if (_vbo != 255)
	{
		// Unbind buffer set to 'GL_ARRAY_BUFFER' target, in
		//	case it's the '_vbo'.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &_vbo);
		_vbo = 255; // 255 means invalid value.
	}

	if (_ebo != 255)
	{
		// Unbind buffer set to 'GL_ELEMENT_ARRAY_BUFFER' target, in
		//	case it's the '_vbo'.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &_ebo);
		_ebo = 255; // 255 means invalid value.
	}

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &_vao);
	_vao = 255;	// 255 means invalid value.
}

void Model::PushVertexAttribute(VertexAttribute& attribute, unsigned int location)
{
	GLint current_size = 0;
	if (_vbo != 255)
	{
		glBindBuffer(GL_COPY_READ_BUFFER, _vbo);
		glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &current_size);
	}

	unsigned int attribute_data_size = sizeof(float) * attribute.count;
	GLint new_size = current_size + attribute_data_size;

	GLuint new_vbo;
	glGenBuffers(1, &new_vbo);
	glBindBuffer(GL_COPY_WRITE_BUFFER, new_vbo);
	glBufferData(GL_COPY_WRITE_BUFFER, new_size, nullptr, GL_STATIC_DRAW);

	// If there's no data to copy from '_vbo' (bound to 'GL_COPY_READ_BUFFER',
	//	skip the copying process. Otherwise, copy everything from '_vbo' into
	//	the new buffer object.
	if (current_size != 0)
	{
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, current_size);
	}

	glBufferSubData(GL_COPY_WRITE_BUFFER, current_size, attribute_data_size,
		attribute.data.data());

	// Unbind the Read and Write targets.
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

	// Delete the old '_vbo' object handle,
	//	if it was in use.
	if (_vbo != 255)
	{
		glDeleteBuffers(1, &_vbo);
	}
	_vbo = new_vbo;

	// Bind the Array target and VAO.
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// Save the 'attribute' object's configuration
	//	data into a new VertexAttributeLayout object.
	_attribute_layouts.push_back(
	{
		attribute,
		location,
		(unsigned int) sizeof(float) * attribute.dimension,
		(unsigned int) current_size
	});

	// Set up all the VertexAttribPointers necessary.
	for (auto& layout : _attribute_layouts)
	{
		unsigned int index = layout.location;
		unsigned int size = layout.attribute.dimension;
		unsigned int stride = layout.stride;
		unsigned int offset = layout.offset;

		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (const void*) offset);
		glEnableVertexAttribArray(index);
	}

	// Unbind VAO after setting all the data.
	glBindVertexArray(0);

	_number_of_vertices = (GLuint) attribute.count / attribute.dimension;
}

void Model::SetElementArrayBuffer(VertexAttribute& attribute)
{
	// Convert 'float' values into 'unsigned int' values for
	//	GL_ELEMENT_ARRAY_BUFFER buffer to work properly.
	std::vector<GLuint> indices;
	for (size_t i=0; i<attribute.count; i++)
	{
		indices.push_back(attribute.data.at(i));
	}

	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, attribute.count * sizeof(unsigned int),
		indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Model::Draw() const
{
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _number_of_vertices);
	glBindVertexArray(0);
}

void Model::DrawVolume() const
{
	glBindVertexArray(_vao);
	// TODO: This is very ugly. Make it better later.
	//	'vertices_count' should equal '72' for cube shaped objects.
	const GLsizei vertices_count = _number_of_vertices * 9;
	glDrawElements(GL_TRIANGLES_ADJACENCY, vertices_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Model::RecalculateModelMatrix()
{
	_model = _translation * _rotation * _scale;
}

void Model::SetScale(const glm::vec3& scaleVector)
{
	_scale = glm::scale(glm::mat4(1.0f), scaleVector);
	RecalculateModelMatrix();
}

void Model::SetRotation(glm::vec3 vector, float angle)
{
	_rotation = glm::rotate(angle, vector);	// 'GLM EXPERIMENTAL'
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
