/*
 * CommandLine.cpp
 *
 *  Created on: 2015¦~4¤ë1¤é
 *      Author: LukeWu
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string>
#include "CommandLine.h"

CommandLine::CommandLine(float _x, float _y, std::string _text) :Label( _x, _y, _text) {
	obj_vertex.clear();
	obj_vertex.push_back(x+0);  obj_vertex.push_back(y+0);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+840);  obj_vertex.push_back(y+0);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+840);  obj_vertex.push_back(y+0);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+840);  obj_vertex.push_back(y+40);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+840);  obj_vertex.push_back(y+40);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+0);  obj_vertex.push_back(y+40);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+0);  obj_vertex.push_back(y+40);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+0);  obj_vertex.push_back(y+0);  obj_vertex.push_back(0.0);

}

CommandLine::~CommandLine(){}


void CommandLine::render(Renderer* renderer, bool view_changed){
	if(!visible || !active)return;

	float _x, _y, _z;
	float t;
	float pixel_x, pixel_y, pixel_dis;
	glm::vec3 dir, projected_position;
	glm::vec4 modelspace_position;

	if(!view_changed){
		//printf("not changed~\n");
		renderer->useProgram(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_projected_vertex);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		renderer->setUniform(model_matrix ,mode);
		glDrawArrays(GL_POINTS, 0, projected_vertices.size());
		return;
	}
	//printf("changed~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	projected_vertices.clear();
	std::vector<float> filled_projected_vertices;

	for(unsigned int i=0; i<obj_vertex.size(); i+=3){

		int window_height = renderer->getWindow()->getWindowHeight();
		int window_width = renderer->getWindow()->getWindowWidth();
		_x = obj_vertex[i];
		_y = obj_vertex[i+1];
		_z = obj_vertex[i+2];

		projected_vertices.push_back(_x);
		projected_vertices.push_back(_y);
		projected_vertices.push_back(_z);

		std::vector<float> v(projected_vertices);
		renderer -> DrawLine(v, mode);

	}

	drawText(command.c_str(), (int)x+20, (int)y+10);
	std::vector<float> v(projected_vertices);
	renderer -> DrawLine(v, mode);

}

void CommandLine::keyActive(int key, int action){

	if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){
		if(!active)active = true;
		else{
			command = "";
			active = false;
		}
	}
	else if(action == GLFW_PRESS && active){
		if(key>='A' && key<='Z')command += (char)(key-'A'+'a');
		else command += (char)(key);
	}
}



