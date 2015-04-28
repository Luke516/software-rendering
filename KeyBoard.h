/*
 * KeyBoard.h
 *
 *  Created on: 2014/7/30
 *      Author: LukeWu
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_
#include "BasicObject.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class KeyBoard {
public:
	KeyBoard();
	void update();
	void keyActive(int key, int action);
	void mouseActive(int button, glm::vec3 pos, glm::vec3 dir, int x_pos, int y_pos );
	void push(BasicObject *np);
	void clear();
private:
	std::vector<BasicObject*>keyboard_list;
};

#endif /* KEYBOARD_H_ */
