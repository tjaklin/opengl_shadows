#include "../inc/Model.hpp"

Model::Model(const float* vbo, const int vbo_size)
{
	// TODO: [Refactoring] Necu da mi se u ovu metodu salje pointer na
	// objekte iz 'meshVertices.cpp' jer me to tjera na plač. Umjesto
	// toga, hocu napisati neku klasicu koja bi se bavila citanjem
	// '.txt' fajlova (u kojima bi bili zadani vertex attributi za polozaj
	// tocaka nekog oblika). Ta klasica bi procitane podatke punila u neki
	// float array (ili slicno) i vratila bi taj array nazad nama nakoristenje.
	// Onda se taj array posalje ko argument ovoj tu metodi. S tim sam dobil da
	// se lako moze zadati novi oblik kroz .txt fajl i nemora se nist rekompa-
	// jlirati (nego se samo ponovno pokrene program).
	// DODATNO, nekaj takvog vec imam za citanje 'Shader-a' ??

	if (!vbo) { return; }
	// TODO: Kak da se implementira 'throw' iz konstruktora, u slucajevima
	//	da dojde do greske u samome konstruktoru ?
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vbo_size, (const void*) vbo, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Unbind VAO after setting all the data.
	glBindVertexArray(0);

	_number_of_vertices = (GLuint) vbo_size / 3;
}

Model::~Model()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	_vao = 0;
	_vbo = 0;
}

void Model::Draw(unsigned int attributes) const
{
	// Currently, only a single Vertex Attribute is supported and that is Position.
	glBindVertexArray(_vao);

	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, _number_of_vertices);
	glDisableVertexAttribArray(0);

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
