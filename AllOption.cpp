/*
 * AllOption.cpp
 *
 *  Created on: 2015¦~3¤ë26¤é
 *      Author: LukeWu
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glmmm.h"
#include "GlobalLoaders.h"
#include "AllOption.h"

RotateOption::RotateOption(float _x, float _y, std::string _text, glm::vec3 v)
	: Label(_x, _y, _text),rotate_vector(v)
{
	active_flag = false;
	direction = true;
}

RotateOption::~RotateOption() {
}

float RotateOption::mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos){
	if(x_pos>=x && x_pos<=x+200 && y_pos>=y && y_pos<=y+40){
		if(x_pos <= x+100){
			direction = false;
		}
		else{
			direction = true;
		}
		return 0.1;
	}
	else return -1.0f;
}

void RotateOption::keyActive(int key, int action){
	if(key == 'R'){
		if(action == GLFW_PRESS)active_flag = true;
		if(action == GLFW_RELEASE)active_flag = false;
	}
}

void RotateOption::update(){
	if(getTarget() != NULL && active_flag)getTarget() -> rotate(1.0,rotate_vector);
}

void RotateOption::setMode(int _mode){
	mode = _mode;
	if(mode == 2){
		if(getTarget() != NULL){
			if(direction)getTarget() -> rotate(5.0,rotate_vector);
			else getTarget() -> rotate(-5.0,rotate_vector);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////


TranslateOption::TranslateOption(float _x, float _y, std::string _text, glm::vec3 v)
	: Label(_x, _y, _text),translate_vector(v)
{
	active_flag = false;
	direction = true;
}

TranslateOption::~TranslateOption() {
}

float TranslateOption::mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos){
	if (x_pos >= x && x_pos <= x + 200 && y_pos >= y && y_pos <= y + 40) {
		if (x_pos <= x + 100) {
			direction = false;
		} else {
			direction = true;
		}
		return 0.1;
	}
	else return -1.0f;
}

void TranslateOption::keyActive(int key, int action){
	if (key == 'T') {
		if(action == GLFW_PRESS)active_flag = true;
		if(action == GLFW_RELEASE)active_flag = false;
	}
}

void TranslateOption::update(){
	if (getTarget() != NULL && active_flag)getTarget()->translate(translate_vector);
}

void TranslateOption::setMode(int _mode){
	mode = _mode;
	if(mode == 2){
		if(getTarget() != NULL){
			if(direction)getTarget() -> translate(translate_vector);
			else getTarget() -> translate(-translate_vector);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////


ScaleOption::ScaleOption(float _x, float _y, std::string _text, glm::vec3 v)
	: Label(_x, _y, _text),scale_vector(v)
{
	active_flag = false;
	direction = true;
}

ScaleOption::~ScaleOption() {
}

float ScaleOption::mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos){
	if (x_pos >= x && x_pos <= x + 200 && y_pos >= y && y_pos <= y + 40) {
		if (x_pos <= x + 100) {
			direction = false;
		} else {
			direction = true;
		}
		return 0.1;
	}
	else return -1.0f;
}

void ScaleOption::keyActive(int key, int action){

}

void ScaleOption::update(){

}

void ScaleOption::setMode(int _mode){
	mode = _mode;
	if(mode == 2){
		if(getTarget() != NULL){
			if(direction)getTarget() -> scale(scale_vector);
			else getTarget() -> scale(glm::vec3(1/scale_vector.x, 1/scale_vector.y, 1/scale_vector.z));
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////


MirrorOption::MirrorOption(float _x, float _y, std::string _text, glm::vec3 v)
	: Label(_x, _y, _text),mirror_vector(v)
{
	active_flag = false;
	direction = true;
}

MirrorOption::~MirrorOption() {
}

float MirrorOption::mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos){
	if (x_pos >= x && x_pos <= x + 200 && y_pos >= y && y_pos <= y + 40) {
		if (x_pos <= x + 100) {
			direction = false;
		} else {
			direction = true;
		}
		return 0.1;
	}
	else return -1.0f;
}

void MirrorOption::keyActive(int key, int action){

}

void MirrorOption::update(){

}

void MirrorOption::setMode(int _mode){
	mode = _mode;
	if(mode == 2){
		if(getTarget() != NULL){
			getTarget() -> mirror(mirror_vector);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////


ShearOption::ShearOption(float _x, float _y, std::string _text, glm::vec3 v)
	: Label(_x, _y, _text),shear_vector(v)
{
	active_flag = false;
	direction = true;
}

ShearOption::~ShearOption() {
}

float ShearOption::mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos){
	if (x_pos >= x && x_pos <= x + 200 && y_pos >= y && y_pos <= y + 40) {
		if (x_pos <= x + 100) {
			direction = false;
		} else {
			direction = true;
		}
		return 0.1;
	}
	else return -1.0f;
}

void ShearOption::keyActive(int key, int action){

}

void ShearOption::update(){

}

void ShearOption::setMode(int _mode){
	mode = _mode;
	if(mode == 2){
		if(getTarget() != NULL){
			if(direction)getTarget() -> shear(shear_vector);
			else getTarget() -> shear(-shear_vector);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////

