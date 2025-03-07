#include<GL/glew.h>
#include<iostream>
#include<vector>
#include<fstream>

#ifndef LOADSHADER_H
#define LOADSHADER_H

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path, const char * geometry_file_path);
#endif