/*
 * MyShader.cpp
 *
 *  Created on: 2015¦~3¤ë12¤é
 *      Author: LukeWu
 */

#include "MyShader.h"

GLuint MyShader::CreateShaderFromFile(const char *File,GLenum ShaderType)
{
	GLuint ShaderID = glCreateShader(ShaderType);

	std::ifstream fin;
	fin.open(File);
	if(!fin.is_open()){
		fin.close();
		std::cout<<"File not found!"<<File<<std::endl;
		return 0;
	}

	std::string Source((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	fin.close();
	const char *ShaderSource = Source.c_str();

	glShaderSource(ShaderID,1,&ShaderSource,NULL);
	glCompileShader(ShaderID);

	GLint CompileStatus;
	glGetShaderiv(ShaderID,GL_COMPILE_STATUS,&CompileStatus);

	if(CompileStatus != GL_TRUE){
		std::cout<<"compile fail! "<<File<<std::endl;
		GLint InfoLogLength;
		glGetShaderiv(ShaderID,GL_INFO_LOG_LENGTH,&InfoLogLength);
		char *InfoLog = new char[InfoLogLength+1];
		glGetShaderInfoLog(ShaderID,InfoLogLength+1,NULL,InfoLog);
		std::cout<<InfoLog<<std::endl;
		delete InfoLog;
		return 0;
	}
	return ShaderID;
}
