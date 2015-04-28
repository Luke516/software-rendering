/*
 * SelectableObject.h
 *
 *  Created on: 2014/10/23
 *      Author: LukeWu
 */

#ifndef SELECTABLEOBJECT_H_
#define SELECTABLEOBJECT_H_

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"

class SelectableObject {
public:
	SelectableObject();
	virtual ~SelectableObject();
	virtual float mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos);
	virtual void setMode(int button);
	virtual int getMode(){return mode;}
	virtual void render(Renderer* renderer, bool view_changed);
	virtual void update();
	virtual void keyActive(int key, int action);
protected:
	int mode;// 0->none 1->touched 2->selected
	glm::vec3 aabb_bottom_left, aabb_top_right,aabb_bottom_left_model, aabb_top_right_model;
	float x, y, z;
	//int mode;// 0->none 1->touched 2->selected
	std::vector<float> obj_vertex;
	std::vector<float> obj_uv;
	std::vector<float> obj_normal;
	//glm::vec3 aabb_bottom_left, aabb_top_right,aabb_bottom_left_model, aabb_top_right_model;
	glm::mat4 model_matrix;
	glm::mat4 translate_matrix;
	glm::mat4 rotate_matrix;
	glm::mat4 scale_matrix;
	GLuint vbo_vertex, vbo_uv, vbo_normal, vbo_projected_vertex;
};

#endif /* SELECTABLEOBJECT_H_ */
