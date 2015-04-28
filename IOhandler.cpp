/*
 * IOhandler.cpp
 *
 *  Created on: 2014/8/19
 *      Author: LukeWu
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <iostream>
#include "IOhandler.h"


IOhandler::IOhandler(MyWindow *w):renderer(w)
{
	window_ptr = w;
}

void IOhandler::clear(){
	//renderer.clear();
	//keyboard.clear();
}
void IOhandler::init(){
	renderer.init();
	//keyboard.init();
}
void IOhandler::push(BasicObject *np){
	renderer.push(np);
	keyboard.push(np);
}
void IOhandler::render(){
	renderer.render();
}
void IOhandler::update(){
	keyboard.update();
}
void IOhandler::keyActive(int key, int action){
	printf("key = %d = %c\n",key,key);
	if(key == 'W' || key == 'A' || key == 'S' || key == 'D' || key == 'Q' || key == 'E'){
		if(action != GLFW_RELEASE)renderer.keyActive(key);
	}
	keyboard.keyActive(key, action);
}
void IOhandler::mouseActive(int button, double x_pos, double y_pos){
	if(button == 4 ){
		renderer.cameraScale(2);
		return;
	}
	else if (button == -4) {
		renderer.cameraScale(-2);
		return;
	}

	else if(button == 1){
		renderer.cameraTranslate(-0.1*x_pos,-0.1*y_pos);
		keyboard.mouseActive(button,glm::vec3(0,0,0),glm::vec3(0,0,0),x_pos,y_pos);
	}
	else if(button == 2){
		keyboard.mouseActive(button,glm::vec3(0,0,0),glm::vec3(0,0,0),x_pos,y_pos);
	}
	else if(button == 3){
		renderer.cameraRotate(x_pos,y_pos);
	}
	else if(button == 0){

		//printf("x:%f y:%f\n",x_pos,y_pos);
		int window_height = window_ptr->getWindowHeight();
		int window_width = window_ptr->getWindowWidth();
		//printf("window_height: %d  window_width: %d\n",window_height,window_width);
		GLfloat x = (x_pos / window_width) * 2 - 1;
		GLfloat y = (y_pos / window_height) * 2 - 1;	///??!!
		//printf("x=%f y=%f\n",x,y);
		GLfloat z_near = renderer.getZnear();
		GLfloat rad = renderer.getFovy() / 180 * 3.1415926;

		glm::vec3 camera_up = renderer.getCameraUp();
		glm::vec3 camera_position = renderer.getCameraPosition();
		glm::vec3 camera_look_at_posiotion = renderer.getCameraLookAtPosition();
		glm::vec3 view = camera_look_at_posiotion - camera_position;
		glm::vec3 w = glm::cross(view, camera_up);
		glm::vec3 h = glm::cross(view, w);

		w = glm::normalize(w);
		h = glm::normalize(h);
		//printf("w: %f, %f, %f\n",w[0],w[1],w[2]);
		//printf("h: %f, %f, %f\n",h[0],h[1],h[2]);

		view = glm::normalize(view);

		GLfloat h_length = tan(rad / 2) * z_near;
		GLfloat w_length = h_length * window_width / window_height;
		h = h * h_length;
		w = w * w_length;
		glm::vec3 pos = camera_position + view * z_near + h * y + w * x;
		glm::vec3 dir = pos - camera_position;
		//printf("pos: %f,%f,%f  dir:%f %f %f\n",pos[0],pos[1],pos[2],dir[0],dir[1],dir[2]);

		keyboard.mouseActive(button, pos, dir, x_pos, y_pos);
	}
}
