/*
 * KeyBoard.cpp
 *
 *  Created on: 2014/7/30
 *      Author: LukeWu
 */

#include "KeyBoard.h"

KeyBoard::KeyBoard() {

}

void KeyBoard::update(){
	for (unsigned int i = 0; i < keyboard_list.size(); i++) {
		keyboard_list[i]->update();
	}
	return;
}

void KeyBoard::push(BasicObject* np){
	keyboard_list.push_back(np);
}

void KeyBoard::mouseActive(int button,glm::vec3 pos,glm::vec3 dir, int x_pos, int y_pos){
	for (unsigned int i = 0; i < keyboard_list.size(); i++) {
		keyboard_list[i]->mouseActive(button, pos, dir, x_pos, y_pos);
	}
	return;
}


void KeyBoard::keyActive(int key, int action){
	for (unsigned int i = 0; i < keyboard_list.size(); i++) {
		keyboard_list[i]-> keyActive(key, action);
	}
}

