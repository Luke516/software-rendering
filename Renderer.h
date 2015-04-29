/*
 * Renderer.h
 *
 *  Created on: 2015¦~3¤ë12¤é
 *      Author: LukeWu
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "BasicObject.h"
#include "MyWindow.h"
#include "Light.h"

class Renderer {
public:
	Renderer(MyWindow *w);
	void init();
	void render();
	void clear();
	void setUniform(glm::mat4 model_matrix,int mode=0, bool three_d=true);
	void push(BasicObject *np);
	void useProgram(unsigned int id);
	void resize();
	void cameraScale(float dis);
	void cameraTranslate(float x_dis, float y_dis);
	void cameraRotate(int parallel,int vertical );//not complete
	void keyActive(int key);

	void DrawTriangle(std::vector<float> &pixel_position, int mode);
	void DrawLine(std::vector<float> &pixel_position, int mode);
	void DrawPixel(int x, int y, float t, int color_r=0, int color_g=0, int color_b=0);

	void findLookAtMatrix();

	unsigned int createShaderProgram(const char* vert_path, const char* frag_path);

	MyWindow* getWindow(){return window;}

	glm::vec3 getCameraPosition(){return camera_position;}
	glm::vec3 getCameraLookAtPosition(){return camera_lookat_position;}
	glm::vec3 getCameraUp(){return camera_up;}
	glm::mat4 getViewMatrix(){return view_matrix;}
	GLfloat getFovy(){return fovy;}
	GLfloat getZnear(){return z_near;}

	char *getPixelData(){return screen_pixel_data;}

	void pushLight(Light l){light_list.push_back(l);}
	std::vector<Light>& getLightList(){return light_list;}
	void setRenderLightList(std::vector<Light> &list){light_list = list;}
	std::vector<Light>& getRenderLightList(){return light_list;}
private:
	bool view_changed;
	MyWindow* window;
	GLfloat fovy;
	GLfloat z_near, z_far;
	GLfloat light;
	GLfloat window_proportion;
	glm::vec3 camera_position;
	glm::vec3 camera_lookat_position;
	glm::vec3 camera_up;
	std::vector<BasicObject*>render_list;
	std::vector <Light> &light_list;
	std::vector<unsigned int>shader_program_list;
	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 pv_matrix;
	unsigned int matrix_location;
	unsigned int model_matrix_location;
	unsigned int view_matrix_location;
	unsigned int light_location;
	unsigned int screen_texture_id, texture_unif_location;
	unsigned int vbo_vertex, vbo_uv;
	unsigned int pixel_color;
	char *screen_pixel_data;
	float vertices[12], uvs[8];
	int triangle_bound[1500][2];
	float bound_depth[1500][2];
	int window_width;
	int window_height;
	int screen_size;
	float *z_buffer;
};


#endif /* RENDERER_H_ */
