/*
 * BasicObject.h
 *
 *  Created on: 2015¦~3¤ë16¤é
 *      Author: LukeWu
 */

#ifndef BASICOBJECT_H_
#define BASICOBJECT_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer;

class BasicObject {
public:
	BasicObject(){}
	virtual ~BasicObject(){}
	virtual void render(Renderer *renderer, bool view_changed)=0;
	virtual void update()=0;
	virtual void keyActive(int key, int action)=0;
	virtual void mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos)=0;
};

#endif /* BASICOBJECT_H_ */
