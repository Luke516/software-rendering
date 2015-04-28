/*
 * Label.h
 *
 *  Created on: 2015¦~3¤ë20¤é
 *      Author: LukeWu
 */

#ifndef LABEL_H_
#define LABEL_H_

#include <string>
#include"SelectableObject.h"
#include"Model.h"

class Label : public SelectableObject{
public:
	Label(float _x, float _y, std::string _text);
	virtual ~Label();
	virtual void render(Renderer* renderer, bool view_changed);
	virtual void update();
	virtual void keyActive(int key, int action);
	virtual float mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos);
	void drawText(const char text[], int w, int h);
	void swapVisible(){visible = !visible;}
	static void setTarget(Model *_target){target = _target;}
	static Model *getTarget(){return target;}
protected:
	bool visible;
	std::string text;
	std::vector<float> projected_vertices;
	static Model *target;
};

#endif /* LABEL_H_ */
