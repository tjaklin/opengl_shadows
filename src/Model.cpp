#include "model.h"

Model::Model(){

}

void Model::drawArray( uint attributes){
switch( attributes ){

case 3:
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*) (6*sizeof(GLfloat)) );

default:	
case 2:
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*) (3*sizeof(GLfloat)) );
case 1:
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0 );
	};
glDrawArrays(GL_TRIANGLES, 0, mNUM_VERTICES );
glDisableVertexAttribArray(0);
glDisableVertexAttribArray(1);
glDisableVertexAttribArray(2);
}

void Model::drawElements(){
	// 1st attribute buffer : vertices
glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, mVBO);
glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	// Draw the triangles !
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
glDrawElements(GL_TRIANGLES_ADJACENCY, 72, GL_UNSIGNED_INT, 0);
glDisableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::drawModel(uint attributes){

   if( isVolume )
	drawElements();
   else
	drawArray( attributes );
}

bool Model::fillData(const GLfloat* vbo, const int vbo_size)
{
	if( !vbo )
		return false;

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_size, vbo, GL_STATIC_DRAW);

	mNUM_VERTICES = vbo_size/32;

glBindBuffer(GL_ARRAY_BUFFER, 0);

if(mVBO == 0)
	return false;
return true;
}

bool Model::fillData(const GLfloat* vbo, const int vbo_size, 
			const GLuint* ebo, const int ebo_size){

	isVolume = true;
	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_size, ebo, GL_STATIC_DRAW);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_size, vbo, GL_STATIC_DRAW);

	mNUM_VERTICES = vbo_size/32;

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

if( mVBO == 0 || mEBO == 0)
	return false;
return true;
}

void Model::setScale( float factor ){
	mScale     = glm::scale( glm::mat4(1.0f), glm::vec3(factor) );
}
void Model::setRotate( glm::vec3 vector, float angle ){
	mRotate    = glm::rotate( angle, vector);
}
void Model::setTranslate( glm::vec3 vector ){
	mTranslate = glm::translate( glm::mat4(1.0f), vector);
}
void Model::setModel(){
	mModel = mTranslate * mRotate * mScale;
}

const GLfloat* Model::getScale() { return &mScale[0][0]; }
const GLfloat* Model::getRotate() { return &mRotate[0][0]; }
const GLfloat* Model::getTranslate() { return &mTranslate[0][0]; }
const GLfloat* Model::getModelFLOAT() { return &mModel[0][0]; }
glm::mat4 Model::getModelMAT() { return mModel; }
