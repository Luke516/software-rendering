/*
 * Model.cpp
 *
 *  Created on: 2015¦~3¤ë16¤é
 *      Author: LukeWu
 */


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glmmm.h"
#include "Model.h"
#include "Label.h"
#include "GlobalLoaders.h"


Model::Model(float _x, float _y, float _z, std::string file_name):rotate_center(0.0)
{
	visible = true;
	perspective = true;
	x = _x;
	z = _z;
	y = _y;

	//ObjLoader::LoadObjFromFileWithAABB(file_name.c_str(), &obj_vertex, &obj_uv, &obj_normal,
		//	&aabb_bottom_left, &aabb_top_right);

	ObjLoader::LoadObjFromFileVertexAndNormal(file_name.c_str(), &obj_vertex, &obj_normal, &aabb_bottom_left, &aabb_top_right);

	projected_vertices = std::vector<float>(obj_vertex);

	glGenBuffers(1, &vbo_projected_vertex);
	/*glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_uv);
	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, obj_vertex.size() * sizeof(float),&obj_vertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, obj_normal.size() * sizeof(float),&obj_normal[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, obj_uv.size() * sizeof(float), &obj_uv[0],GL_STATIC_DRAW);*/

	if (x || y || z) {
		glm::vec3 translate_vector(x, y, z);
		translate(translate_vector);
		//model_matrix = glm::rotate(model_matrix, 60.0f, glm::vec3(0, 1, 0));
		//model_matrix = glm::rotate(model_matrix, 300.0f, glm::vec3(1, 0, 0));
		//model_matrix = glm::translate(model_matrix, translate_vector);//glm::vec3(2,0,0));
		/*glm::vec4 bl(aabb_bottom_left,1.0);
		glm::vec4 tr(aabb_top_right,1.0);

		bl = model_matrix * bl;
		tr = model_matrix * tr;
		aabb_bottom_left_model = glm::vec3(bl[0],bl[1],bl[2]);
		aabb_top_right_model = glm::vec3(tr[0],tr[1],tr[2]);*/
	}
}

Model::~Model(){}

void Model::render(Renderer* renderer, bool view_changed){
	if(!visible)return;

	if(!perspective){
		renderOtho(renderer);
		return;
	}

	char* data = renderer->getPixelData();

	bool clip = false;
	float _x, _y, _z;
	float t, depth;
	float pixel_x, pixel_y, pixel_dis;
	glm::vec3 dir, projected_position;
	glm::vec4 modelspace_position;

	//printf("changed~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	projected_vertices.clear();
	//std::vector<float> filled_projected_vertices;


	for(unsigned int i=0; i<obj_vertex.size(); i+=3){
		_x = obj_vertex[i];
		_y = obj_vertex[i+1];
		_z = obj_vertex[i+2];

		modelspace_position = renderer->getViewMatrix() * model_matrix * glm::vec4(_x, _y, _z, 1.0);

		int window_height = renderer->getWindow()->getWindowHeight();
		int window_width = renderer->getWindow()->getWindowWidth();
		//printf("window_height: %d  window_width: %d\n",window_height,window_width);
		GLfloat z_near = renderer->getZnear();
		GLfloat rad = renderer->getFovy() / 180 * 3.1415926;

		//glm::vec3 camera_up = glm::vec3(0,1,0);//renderer->getCameraUp();
		glm::vec3 camera_position = glm::vec3(0,0,0);//renderer->getCameraPosition();
		//glm::vec3 camera_look_at_posiotion = glm::vec3(0,0,1);//renderer->getCameraLookAtPosition();

		if(modelspace_position.z < z_near){
			clip = true;
		}
		if(i%9==0 && clip){
			projected_vertices.pop_back();
			projected_vertices.pop_back();
			projected_vertices.pop_back();
			projected_vertices.pop_back();
			projected_vertices.pop_back();
			projected_vertices.pop_back();
			projected_vertices.pop_back();
			projected_vertices.pop_back();
			projected_vertices.pop_back();
			clip = false;
		}

		dir = glm::vec3(modelspace_position.x, modelspace_position.y, modelspace_position.z);
		depth = glm::length(dir);
		//dir = glm::vec3(_x, _y, _z) - camera_position;
		dir = glm::normalize(dir);

		GLfloat h_length = tan(rad / 2) * z_near;
		GLfloat w_length = h_length * window_width / window_height;
		glm::vec3 w = glm::vec3(w_length,0,0);//glm::cross(view, camera_up);
		glm::vec3 h = glm::vec3(0,h_length,0);//glm::cross(view, w);

		glm::vec3 view = glm::vec3(0,0,-z_near);//camera_look_at_posiotion - camera_position;

		glm::vec3 middle_point_of_screen = camera_position + view;

		/*t =  (view.x * dir.x + view.y * dir.y + view.z * dir.z);
		//printf("t = %f\n",t);
		t = ((middle_point_of_screen.x - camera_position.x) * view.x
					+(middle_point_of_screen.y - camera_position.y) * view.y
					+(middle_point_of_screen.z - camera_position.z) * view.z) / t;*/

		t = -z_near/glm::abs(dir.z);

		projected_position = dir * t;

		GLfloat h_offset = glm::dot((projected_position - middle_point_of_screen), h) / glm::dot(h, h);
		GLfloat w_offset = glm::dot((projected_position - middle_point_of_screen), w) / glm::dot(w, w);

		//projected_vertices.push_back((w_offset+1)*window_width/2);
		//projected_vertices.push_back((h_offset+1)*window_height/2);   //why negative QWQ?
		//projected_vertices.push_back(depth);

		//printf("x=%f, y=%f, z=%f, depth=%f\n",modelspace_position.x, modelspace_position.y, modelspace_position.z, depth*100);
		//projected_vertices.push_back(0.1);


		//for(int k=0; k<50000; k++)
			//data[k] = 250;

		/*if((i+3)%9 == 0){ //last vertex of a triangle
			pixel_x = ( projected_vertices[i] - projected_vertices[i-6] ) * window_width;
			pixel_y = ( projected_vertices[i+1] - projected_vertices[i-5] ) * window_height;
			pixel_dis = glm::sqrt(pixel_x*pixel_x + pixel_y*pixel_y);
			for(float f=0.0; f<=pixel_dis; f+=3.0){
				filled_projected_vertices.push_back(projected_vertices[i-6] + (pixel_x/window_width)*f/pixel_dis);
				filled_projected_vertices.push_back(projected_vertices[i-5] + (pixel_y/window_height)*f/pixel_dis);
				filled_projected_vertices.push_back(0.1);
			}
		}
		if((i+3)%9 == 0 || (i+6)%9 == 0){
			pixel_x = ( projected_vertices[i] - projected_vertices[i-3] ) * window_width;
			pixel_y = ( projected_vertices[i+1] - projected_vertices[i-2] ) * window_height;
			pixel_dis = glm::sqrt(pixel_x*pixel_x + pixel_y*pixel_y);
			for(float f=0.0; f<=pixel_dis; f+=3.0){
				filled_projected_vertices.push_back(projected_vertices[i-3] + (pixel_x/window_width)*f/pixel_dis);
				filled_projected_vertices.push_back(projected_vertices[i-2] + (pixel_y/window_height)*f/pixel_dis);
				filled_projected_vertices.push_back(0.1);
			}
		}*/

		//printf("%f, %f, %f\n",projected_vertices[i] ,projected_vertices[i+1], projected_vertices[i+2]);

		/*
		projected_vertices.push_back(projected_position.x);
		projected_vertices.push_back(projected_position.y);
		projected_vertices.push_back(projected_position.z);
		 */
	}

	std::vector<float> v(projected_vertices);
	renderer -> DrawTriangle(v, mode);
	//projected_vertices = std::vector<float>(filled_projected_vertices);


	//renderer->useProgram(0);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_projected_vertex);
	//glBufferData(GL_ARRAY_BUFFER, projected_vertices.size() * sizeof(float),&projected_vertices[0], GL_DYNAMIC_DRAW);

	//for(int j=0; j<1000000; j++){}

	/*glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);*/
	//glBindTexture(GL_TEXTURE_2D,texture_id);
	//printf("mode :%d\n",mode);
	//renderer->setUniform(model_matrix ,mode);
	//glDrawArrays(GL_POINTS, 0, projected_vertices.size()/3);

}

void Model::renderOtho(Renderer* renderer){

	char* data = renderer->getPixelData();

	float _x, _y, _z;
	float t;
	float pixel_x, pixel_y, pixel_dis;
	glm::vec3 dir, projected_position;
	glm::vec4 modelspace_position;


	projected_vertices.clear();

	for(unsigned int i=0; i<obj_vertex.size(); i+=3){
		_x = obj_vertex[i];
		_y = obj_vertex[i+1];
		_z = obj_vertex[i+2];

		modelspace_position = model_matrix * glm::vec4(_x, _y, _z, 1.0) ;//* renderer->getViewMatrix();

		int window_height = renderer->getWindow()->getWindowHeight();
		int window_width = renderer->getWindow()->getWindowWidth();
		//printf("window_height: %d  window_width: %d\n",window_height,window_width);

		glm::vec3 camera_up = renderer->getCameraUp();
		glm::vec3 camera_position = renderer->getCameraPosition();
		glm::vec3 camera_look_at_posiotion = renderer->getCameraLookAtPosition();

		glm::vec3 view = camera_look_at_posiotion - camera_position;

		dir = glm::vec3(modelspace_position.x, modelspace_position.y, modelspace_position.z) - camera_position;
		//dir = glm::vec3(_x, _y, _z) - camera_position;
		//dir = glm::normalize(dir);

		glm::vec3 w = glm::cross(view, camera_up);
		glm::vec3 h = glm::cross(view, w);

		GLfloat h_length = 10*glm::length(view);
		GLfloat w_length = h_length * window_width / window_height;
		h = glm::normalize(h) * h_length;
		w = glm::normalize(w) * w_length;

		view = glm::normalize(view) * (float)glm::length(dir);

		glm::vec3 middle_point_of_screen = camera_position;

		t =  glm::length(view);

		projected_position = glm::vec3(modelspace_position.x, modelspace_position.y, modelspace_position.z) - view;

		GLfloat h_offset = glm::dot((projected_position - middle_point_of_screen), h) / glm::dot(h, h);
		GLfloat w_offset = glm::dot((projected_position - middle_point_of_screen), w) / glm::dot(w, w);

		projected_vertices.push_back((w_offset+1)*window_width/2);
		projected_vertices.push_back((h_offset+1)*window_height/2);   //why negative QWQ?
		projected_vertices.push_back(t);
		//projected_vertices.push_back(0.1);


		//for(int k=0; k<50000; k++)
			//data[k] = 250;

		/*if((i+3)%9 == 0){ //last vertex of a triangle
			pixel_x = ( projected_vertices[i] - projected_vertices[i-6] ) * window_width;
			pixel_y = ( projected_vertices[i+1] - projected_vertices[i-5] ) * window_height;
			pixel_dis = glm::sqrt(pixel_x*pixel_x + pixel_y*pixel_y);
			for(float f=0.0; f<=pixel_dis; f+=3.0){
				filled_projected_vertices.push_back(projected_vertices[i-6] + (pixel_x/window_width)*f/pixel_dis);
				filled_projected_vertices.push_back(projected_vertices[i-5] + (pixel_y/window_height)*f/pixel_dis);
				filled_projected_vertices.push_back(0.1);
			}
		}
		if((i+3)%9 == 0 || (i+6)%9 == 0){
			pixel_x = ( projected_vertices[i] - projected_vertices[i-3] ) * window_width;
			pixel_y = ( projected_vertices[i+1] - projected_vertices[i-2] ) * window_height;
			pixel_dis = glm::sqrt(pixel_x*pixel_x + pixel_y*pixel_y);
			for(float f=0.0; f<=pixel_dis; f+=3.0){
				filled_projected_vertices.push_back(projected_vertices[i-3] + (pixel_x/window_width)*f/pixel_dis);
				filled_projected_vertices.push_back(projected_vertices[i-2] + (pixel_y/window_height)*f/pixel_dis);
				filled_projected_vertices.push_back(0.1);
			}
		}*/

		//printf("%f, %f, %f\n",projected_vertices[i] ,projected_vertices[i+1], projected_vertices[i+2]);

		/*
		projected_vertices.push_back(projected_position.x);
		projected_vertices.push_back(projected_position.y);
		projected_vertices.push_back(projected_position.z);
		 */
	}

	std::vector<float> v(projected_vertices);
	renderer -> DrawTriangle(v, mode);
}

void Model::setMode(int next_mode){
	mode = next_mode;
	if(mode == 2)Label::setTarget(this);
}

void Model::translate(glm::vec3 translate_vector){

	translate_matrix[3].x = translate_vector.x;
	translate_matrix[3].y = translate_vector.y;
	translate_matrix[3].z = translate_vector.z;
	model_matrix =  model_matrix * translate_matrix;
	//return;
	glm::vec4 bl(aabb_bottom_left,1.0);
	glm::vec4 tr(aabb_top_right,1.0);
	bl = model_matrix * bl;
	tr = model_matrix * tr;
	aabb_bottom_left_model = glm::vec3(bl[0],bl[1],bl[2]);
	aabb_top_right_model = glm::vec3(tr[0],tr[1],tr[2]);
	//aabb_top_right +=  translate_vector;
	//aabb_bottom_left += translate_vector;
	//return glm::translate(matrix, translate_vector);
	//model_matrix = glmmm::translate(model_matrix, translate_vector);
}

void Model::rotate(float angle, glm::vec3 axis, glm::vec3 offset){
	glm::mat4 rotate_matrix(1.0);
	float u = axis.x;
	float v = axis.y;
	float w = axis.z;
	angle = angle/360*2*3.14159;
	rotate_matrix[0].x = glm::cos(angle) + (u * u) * (1 - glm::cos(angle));
	rotate_matrix[1].x = u * v * (1 - glm::cos(angle)) + w * glm::sin(angle);
	rotate_matrix[2].x = u * w * (1 - glm::cos(angle)) - v * glm::sin(angle);
	rotate_matrix[0].y = u * v * (1 - glm::cos(angle)) - w * glm::sin(angle);
	rotate_matrix[1].y = glm::cos(angle) + v * v * (1 - glm::cos(angle));
	rotate_matrix[2].y = w * v * (1 - glm::cos(angle)) + u * glm::sin(angle);
	rotate_matrix[0].z = u * w * (1 - glm::cos(angle)) + v * glm::sin(angle);
	rotate_matrix[1].z = v * w * (1 - glm::cos(angle)) - u * glm::sin(angle);
	rotate_matrix[2].z = glm::cos(angle) + w * w * (1 - glm::cos(angle));
	translate(rotate_center);
	model_matrix =  model_matrix * rotate_matrix;
	translate(-rotate_center);
	//model_matrix = glm::rotate(model_matrix, angle, axis);
}

void Model::scale(glm::vec3 scale_vector){
	glm::mat4 scale_matrix(1.0);
	scale_matrix[0].x = scale_vector.x;
	scale_matrix[1].y = scale_vector.y;
	scale_matrix[2].z = scale_vector.z;
	model_matrix =  model_matrix * scale_matrix;
}

void Model::mirror(glm::vec3 mirror_vector){
	if(mirror_vector.x != 0){
		for(unsigned int i=0; i<obj_vertex.size(); i+=3){
			obj_vertex[i] = -obj_vertex[i];
		}
		aabb_bottom_left_model.x += (mirror_vector.x - aabb_bottom_left_model.x)*2;
		aabb_top_right_model.x += (mirror_vector.x - aabb_top_right_model.x )*2;
		std::swap(aabb_bottom_left_model.x, aabb_top_right_model.x);
		model_matrix = glm::translate(model_matrix, glm::vec3((mirror_vector.x-x)*2, 0, 0));
		x+=(mirror_vector.x-x)*2;
	}
	else if (mirror_vector.y != 0) {
		for (unsigned int i = 0; i < obj_vertex.size(); i += 3) {
			obj_vertex[i+1] = -obj_vertex[i+1];
		}
		aabb_bottom_left_model.y += (mirror_vector.y - aabb_bottom_left_model.y)*2;
		aabb_top_right_model.y += (mirror_vector.y - aabb_top_right_model.y )*2;
		std::swap(aabb_bottom_left_model.y, aabb_top_right_model.y);
		model_matrix = glm::translate(model_matrix, glm::vec3(0, (mirror_vector.y-y)*2, 0));
		y+=(mirror_vector.y-y)*2;
	}
	else {
		for (unsigned int i = 0; i < obj_vertex.size(); i += 3) {
			obj_vertex[i+2] = -obj_vertex[i+2];
		}
		aabb_bottom_left_model.z += (mirror_vector.z - aabb_bottom_left_model.z)*2;
		aabb_top_right_model.z += (mirror_vector.z - aabb_top_right_model.z )*2;
		std::swap(aabb_bottom_left_model.z, aabb_top_right_model.z);
		model_matrix = glm::translate(model_matrix, glm::vec3(0, 0, (mirror_vector.z-z)*2));
		z+=(mirror_vector.z-z)*2;
	}
}

void Model::shear(glm::vec3 shear_vector){
	for (unsigned int i = 0; i < obj_vertex.size(); i += 3) {
		if(shear_vector.x == 1 )obj_vertex[i] += obj_vertex[i+2];
		else obj_vertex[i] -= obj_vertex[i+2];
	}
}

void Model::update(){

	return;
}


void Model::keyActive(int key, int action){
	if(key == 'M' && action == GLFW_PRESS){
		mirror(glm::vec3(10,0,0));
	}
	else if(key == 'C' && action == GLFW_PRESS){
		scale(glm::vec3(1.2,1.2,1.2));
	}
}
