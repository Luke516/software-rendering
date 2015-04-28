/*
 * Label.cpp
 *
 *  Created on: 2015¦~3¤ë20¤é
 *      Author: LukeWu
 */


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "Label.h"
#include "GlobalLoaders.h"

Model *Label::target;

Label::Label(float _x, float _y, std::string _text)
{
	visible = true;
	x = _x;
	z = 0.0;
	y = _y;
	text = std::string(_text);
	target = NULL;

	/*ObjLoader::LoadObjFromFileWithAABB(file_name.c_str(), &obj_vertex, &obj_uv, &obj_normal,
			&aabb_bottom_left, &aabb_top_right);
	*/

	obj_vertex.push_back(x+0);  obj_vertex.push_back(y+0);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+180);  obj_vertex.push_back(y+0);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+180);  obj_vertex.push_back(y+0);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+180);  obj_vertex.push_back(y+40);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+180);  obj_vertex.push_back(y+40);  obj_vertex.push_back(0.0);
	obj_vertex.push_back(x+0);  obj_vertex.push_back(y+40);  obj_vertex.push_back(0.0);

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

	/*if (x || y || z) {
		glm::vec3 translate_vector(x, y, z);
		model_matrix = glm::translate(model_matrix, translate_vector);
		glm::vec4 bl(aabb_bottom_left,1.0);
		glm::vec4 tr(aabb_top_right,1.0);

		bl = model_matrix * bl;
		tr = model_matrix * tr;
		aabb_bottom_left_model = glm::vec3(bl[0],bl[1],bl[2]);
		aabb_top_right_model = glm::vec3(tr[0],tr[1],tr[2]);
	}*/
}

Label::~Label(){}

void Label::render(Renderer* renderer, bool view_changed){
	if(!visible)return;

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
		/*
		modelspace_position = model_matrix * glm::vec4(_x, _y, _z, 1.0);


		//printf("window_height: %d  window_width: %d\n",window_height,window_width);
		GLfloat z_near = renderer->getZnear();
		GLfloat rad = renderer->getFovy() / 180 * 3.1415926;

		glm::vec3 camera_up = renderer->getCameraUp();
		glm::vec3 camera_position = renderer->getCameraPosition();
		glm::vec3 camera_look_at_posiotion = renderer->getCameraLookAtPosition();

		glm::vec3 view = camera_look_at_posiotion - camera_position;

		dir = glm::vec3(modelspace_position.x, modelspace_position.y, modelspace_position.z) - camera_position;
		//dir = glm::vec3(_x, _y, _z) - camera_position;
		dir = glm::normalize(dir);

		glm::vec3 w = glm::cross(view, camera_up);
		glm::vec3 h = glm::cross(view, w);

		GLfloat h_length = tan(rad / 2) * z_near;
		GLfloat w_length = h_length * window_width / window_height;
		h = glm::normalize(h) * h_length;
		w = glm::normalize(w) * w_length;

		view = glm::normalize(view) * z_near;

		glm::vec3 middle_point_of_screen = camera_position + view;

		t =  (view.x * dir.x + view.y * dir.y + view.z * dir.z);
		//printf("t = %f\n",t);
		t = ((middle_point_of_screen.x - camera_position.x) * view.x
					+(middle_point_of_screen.y - camera_position.y) * view.y
					+(middle_point_of_screen.z - camera_position.z) * view.z) / t;

		projected_position = camera_position + dir * t;

		GLfloat h_offset = glm::dot((projected_position - middle_point_of_screen), h) / glm::dot(h, h);
		GLfloat w_offset = glm::dot((projected_position - middle_point_of_screen), w) / glm::dot(w, w);
	*/
		projected_vertices.push_back(_x);
		projected_vertices.push_back(_y);
		projected_vertices.push_back(_z);

		std::vector<float> v(projected_vertices);
		renderer -> DrawLine(v, mode);

		/*if((i+3)%12 == 0){ //last vertex of a triangle
			pixel_x = ( projected_vertices[i] - projected_vertices[i-9] ) * window_width;
			pixel_y = ( projected_vertices[i+1] - projected_vertices[i-8] ) * window_height;
			pixel_dis = glm::sqrt(pixel_x*pixel_x + pixel_y*pixel_y);
			for(float f=0.0; f<=pixel_dis; f+=3.0){
				filled_projected_vertices.push_back(projected_vertices[i-9] + (pixel_x/window_width)*f/pixel_dis);
				filled_projected_vertices.push_back(projected_vertices[i-8] + (pixel_y/window_height)*f/pixel_dis);
				filled_projected_vertices.push_back(0.1);
			}
		}*/
		/*if( i != 0){
			pixel_x = ( projected_vertices[i] - projected_vertices[i-3] ) * window_width;
			pixel_y = ( projected_vertices[i+1] - projected_vertices[i-2] ) * window_height;
			pixel_dis = glm::sqrt(pixel_x*pixel_x + pixel_y*pixel_y);
			for(float f=0.0; f<=pixel_dis; f+=3.0){
				filled_projected_vertices.push_back(projected_vertices[i-3] + (pixel_x/window_width)*f/pixel_dis);
				filled_projected_vertices.push_back(projected_vertices[i-2] + (pixel_y/window_height)*f/pixel_dis);
				filled_projected_vertices.push_back(0.1);
			}
		}

		//printf("%f, %f, %f\n",projected_vertices[i] ,projected_vertices[i+1], projected_vertices[i+2]);


		projected_vertices.push_back(projected_position.x);
		projected_vertices.push_back(projected_position.y);
		projected_vertices.push_back(projected_position.z);
		 */
	}

	drawText(text.c_str(), (int)x+20, (int)y+10);
	std::vector<float> v(projected_vertices);
	renderer -> DrawLine(v, mode);

	/*projected_vertices = std::vector<float>(filled_projected_vertices);


	renderer->useProgram(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_projected_vertex);
	glBufferData(GL_ARRAY_BUFFER, projected_vertices.size() * sizeof(float),&projected_vertices[0], GL_DYNAMIC_DRAW);

	//for(int j=0; j<1000000; j++){}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	//glBindTexture(GL_TEXTURE_2D,texture_id);
	//printf("mode :%d\n",mode);
	renderer->setUniform(model_matrix ,mode);
	glDrawArrays(GL_POINTS, 0, projected_vertices.size()/3);*/

}

void Label::drawText(const char text[], int w, int h){
	projected_vertices.clear();
	for(int i=0; i<strlen(text) && i<80; i++){
		switch(text[i]){
		case 'a':
			projected_vertices.push_back(0+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(0+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(0+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(15+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(0+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(10+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			break;
		case 'b':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(0+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(13+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(13+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(13+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			break;
		case 'c':
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(11+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(11+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(11+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
				break;
		case 'd':
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(0+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(13+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(13+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(13+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
				break;
		case 'e':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(14+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(14+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(14+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			break;
		case 'f':
			projected_vertices.push_back(4+w+12*i);  projected_vertices.push_back(0+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(4+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(4+w+12*i);  projected_vertices.push_back(0+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(0+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(8+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(8+h);  projected_vertices.push_back(0.0);
				break;
		case 'g':
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(9+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(9+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(9+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(25+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(25+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(25+h);  projected_vertices.push_back(0.0);
				break;
		case 'h':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(0+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
				break;
		case 'i':
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(7+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(8+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(11+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
				break;
		case 'j':
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(7+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(8+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(11+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(25+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(25+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(25+h);  projected_vertices.push_back(0.0);
				break;
		case 'k':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(0+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(15+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(8+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(15+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
				break;
		case 'l':
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(2+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			break;
		case 'm':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(8+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
				break;
		case 'n':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			break;
		case 'o':
			projected_vertices.push_back(2+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(2+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(2+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(2+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			break;
		case 'p':
				break;
		case 'q':
				break;
		case 'r':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(12+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(15+h);  projected_vertices.push_back(0.0);
			break;
		case 's':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(15+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(15+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(15+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(15+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			break;
		case 't':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(2+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			break;
		case 'u':
				break;
		case 'v':
				break;
		case 'w':
				break;
		case 'x':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			break;
		case 'y':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(5+w+12*i);  projected_vertices.push_back(18+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(26+h);  projected_vertices.push_back(0.0);
			break;
		case 'z':
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(10+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(1+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			projected_vertices.push_back(9+w+12*i);  projected_vertices.push_back(20+h);  projected_vertices.push_back(0.0);
			break;
		}
	}
}

void Label::update(){

	return;
}


void Label::keyActive(int key, int action){

}

float Label::mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos){
	if(x_pos>=x && x_pos<=x+180 && y_pos>=y && y_pos<=y+40)return 0.1;
	else return -1.0f;
}
