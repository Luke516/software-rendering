/*
 * MyShader.h
 *
 *  Created on: 2015¦~3¤ë12¤é
 *      Author: LukeWu
 */

#include<iostream>
#include<fstream>
#include<streambuf>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#ifndef MYSHADER_H_
#define MYSHADER_H_

class MyShader {
public:
	static GLuint CreateShaderFromFile(const char *File,GLenum shaderType);
};

#endif /* MYSHADER_H_ */
