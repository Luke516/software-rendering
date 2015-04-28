/*
 * SelectableObject.cpp
 *
 *  Created on: 2014/10/23
 *      Author: LukeWu
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "GlobalLoaders.h"
#include "SelectableObject.h"

SelectableObject::SelectableObject():
	mode(0),
	model_matrix(glm::mat4(1.0f)),
	translate_matrix(1.0),
	rotate_matrix(1.0),
	scale_matrix(1.0),
	vbo_vertex(0),vbo_uv(0),vbo_normal(0)
 {
}

SelectableObject::~SelectableObject(){}

void SelectableObject::setMode(int button){
	printf("set mode : %d\n",button);
	mode = button;
}

float SelectableObject::mouseActive(int button, glm::vec3 pos, glm::vec3 dir, int x_pos, int y_pos){
	float t;
	float tx1 = (aabb_bottom_left_model.x - pos.x)/dir.x;
	float tx2 = (aabb_top_right_model.x - pos.x)/dir.x;
	float ty1 = (aabb_bottom_left_model.y - pos.y)/dir.y;
	float ty2 = (aabb_top_right_model.y - pos.y)/dir.y;
	float tz1 = (aabb_bottom_left_model.z - pos.z)/dir.z;
	float tz2 = (aabb_top_right_model.z - pos.z)/dir.z;
	float tmin = glm::max(glm::max(glm::min(tx1, tx2), glm::min(ty1, ty2)), glm::min(tz1, tz2));
	float tmax = glm::min(glm::min(glm::max(tx1, tx2), glm::max(ty1, ty2)), glm::max(tz1, tz2));
	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if (tmax < 0)
	{
	    t = tmax;
	    return -1;
	}
	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
	    t = tmax;
	    return -1;
	}

	t = tmin;
	return t;
}

void SelectableObject::render(Renderer* renderer, bool view_changed){
	/*renderer->useProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	//glBindTexture(GL_TEXTURE_2D, texture_id);
	renderer->setUniform(model_matrix, mode);
	glDrawArrays(GL_TRIANGLES, 0, obj_vertex.size());*/
}

void SelectableObject::update(){

	return;

}
void SelectableObject::keyActive(int key, int action){
	return;
}

