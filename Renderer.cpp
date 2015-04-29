/*
 * Renderer.cpp
 *
 *  Created on: 2015¦~3¤ë12¤é
 *      Author: LukeWu
 */


#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include "Renderer.h"
#include "MyShader.h"

Renderer::Renderer(MyWindow* w): pixel_color(0),
	vertices({-1.0f, 1.0f, 0.5f,
		1.0f, 1.0f, 0.5f,
		1.0f, -1.0f, 0.5f,
		-1.0f, -1.0f, 0.5f}),
	uvs(
		{0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f}
	),
	screen_texture_id(1),screen_pixel_data(NULL),
	fovy(60.0),z_near(1.0),z_far(1000.0f),light(5),window_proportion(4.0f / 3.0f),
	camera_position(0,20,10),camera_lookat_position(0,10,0),camera_up(0,1,0)
{
	pv_matrix = projection_matrix*view_matrix;
	window = w;

	GLfloat rad = fovy / 180 * 3.1415926;
	float threeD_height = z_near * tan(rad/2) * 2;
	float threeD_width = threeD_height * window_width / window_height;
	window -> setWindowHeight3D(threeD_height);
	window -> setWindowWidth3D(threeD_width);

	//window_proportion = w->getWindowWidth()/w->getWindowHeight(); -- int!!!
	//projection_matrix = glm::perspective(fovy, window_proportion, z_near, 500.0f);
}

void Renderer::init() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	createShaderProgram("VertexShader.vert", "FragmentShader.frag");
	createShaderProgram("VertexShader2.vert", "FragmentShader2.frag");
	//matrix_location = glGetUniformLocation(shader_program_list[0], "MVP");
	//light_location = glGetUniformLocation(shader_program_list[0], "light");
	texture_unif_location = glGetUniformLocation(shader_program_list[0],"myTextureSampler");

	glGenBuffers(1, &vbo_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float),vertices, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &vbo_uv);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float),uvs, GL_DYNAMIC_DRAW);

	findLookAtMatrix();

	window_width = window->getWindowWidth();
	window_height = window->getWindowHeight();
	screen_size = window_width * window_height;
	screen_pixel_data = new char[ window_width * window_height * 3];
	memset(screen_pixel_data, 0, window_width * window_height * 3);

	z_buffer = new float[ window_width * window_height];
	memset(z_buffer, 0, window_width * window_height);

	memset(triangle_bound, 0, 1500*2*sizeof(int));
	memset(bound_depth, 0, 1500*2*sizeof(float) );

	//for(int i=0;i<100000; i++)
		//screen_pixel_data[i] = 100;

	glGenTextures(1, &screen_texture_id);

	glBindTexture(GL_TEXTURE_2D, screen_texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0,3, window_width, window_height, 0, GL_BGR, GL_UNSIGNED_BYTE, screen_pixel_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

unsigned int Renderer::createShaderProgram(const char* vert_path,const char* frag_path) {
	printf("vert_path:%s\nfrag_path:%s\n",vert_path, frag_path);
	GLuint vertex_shader = MyShader::CreateShaderFromFile( vert_path, GL_VERTEX_SHADER);
	GLuint fragment_shader = MyShader::CreateShaderFromFile( frag_path, GL_FRAGMENT_SHADER);

	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	shader_program_list.push_back(shader_program);

	return shader_program_list.size()-1;
}

void Renderer::useProgram(unsigned int id) {
	glUseProgram(shader_program_list[id]);
}

void Renderer::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	memset(screen_pixel_data, 0, sizeof(char)*960*720*3);
	memset(z_buffer, 0, sizeof(float)*960*720);

	for(unsigned int i=0; i<render_list.size(); i++){
		render_list[i] -> render(this, true);
	}

	pixel_color++;
	pixel_color%= 255;

	useProgram(1);
	glBindTexture(GL_TEXTURE_2D, screen_texture_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 960,
			720, GL_BGR, GL_UNSIGNED_BYTE,
			screen_pixel_data);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(texture_unif_location, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_QUADS, 0, 4);

	glfwPollEvents();
	window -> swapBuffer();
	//view_changed = false;
	return;
}

void Renderer::setUniform(glm::mat4 model_matrix, int mode,bool three_d){
	printf("mode = %d\n",mode);
	if(mode == 0)light = 200;
	else if(mode ==1)light = 100;
	else if(mode ==2)light = 10;

	glm::mat4 MVP = pv_matrix * model_matrix;

	//glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, &model_matrix[0][0]);


	if(three_d){
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &MVP[0][0]);
		//glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]);
	}
	else {
		glm::mat4 identity_matrix(1.0);
		MVP = projection_matrix * identity_matrix * model_matrix;
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &MVP[0][0]);
		//glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &identity_matrix[0][0]);
	}

	glUniform1f(light_location, light);
}

void Renderer::push(BasicObject *np){
	render_list.push_back(np);
}

void Renderer::resize(){
	int w = window_width;
	int h = window->getWindowHeight();
	printf("w=%d h=%d\n",w,h);
	glViewport(0,0,w,h);
	window_proportion = (float)w/h;
	projection_matrix = glm::mat4(glm::perspective(fovy, window_proportion, z_near, 100.0f));
	pv_matrix = projection_matrix*view_matrix;

	GLfloat rad = fovy / 180 * 3.1415926;
	float threeD_height = z_near * tan(rad/2) * 2;
	float threeD_width = threeD_height * w / h;
	window -> setWindowHeight3D(threeD_height);
	window -> setWindowWidth3D(threeD_width);

	view_changed = true;
}

void Renderer::cameraScale(float dis){
	glm::vec3 view = camera_lookat_position - camera_position;
	view = glm::normalize(view);
	camera_position = camera_position + view * (GLfloat)(dis);
	findLookAtMatrix();
	//view_matrix = glm::lookAt(camera_position,camera_lookat_position,camera_up);
	pv_matrix = projection_matrix * view_matrix;

	view_changed = true;
}

void Renderer::cameraTranslate(float x_dis, float y_dis){

	glm::vec3 view = camera_lookat_position - camera_position;
	if(view.y == 0)view.y=0.001;
	view = glm::normalize(view);

	float threeD_height = window -> getWindowHeight3D();
	float threeD_width = window -> getWindowWidth3D();

	float far = camera_position.y / view.y; // * glm::length(view); should be 1
	if(far<0)far = -far;
	//printf("far = %f\n",far);

	x_dis = x_dis / window -> getWindowWidth() / z_near * far *3; //* threeD_width;
	y_dis = y_dis / window -> getWindowHeight() / z_near * far *3; //* threeD_height;

	glm::vec3 dir_parallel = glm::cross(view, camera_up);
	dir_parallel.y = 0;
	dir_parallel = glm::normalize(dir_parallel);

	camera_position = camera_position + dir_parallel * x_dis;
	camera_lookat_position = camera_lookat_position + dir_parallel * x_dis;
	//view_matrix = glm::translate(view_matrix, -dir_parallel * dis);

	glm::vec3 dir_vertical = glm::cross(view, dir_parallel);
	dir_vertical.y = 0;
	dir_vertical = glm::normalize(dir_vertical);
	camera_position = camera_position + dir_vertical * y_dis;
	camera_lookat_position = camera_lookat_position + dir_vertical * y_dis;
	//view_matrix = glm::translate(view_matrix, -dir_vertical * dis);

	//view_matrix = glm::lookAt(camera_position,camera_lookat_position,camera_up);
	findLookAtMatrix();
	pv_matrix = projection_matrix * view_matrix;

	view_changed = true;
}

void Renderer::keyActive(int key){
	if(key == 'W')cameraTranslate(0.0, -1.0);
	else if(key == 'S')cameraTranslate(0.0, 1.0);
	else if(key == 'D')cameraTranslate(1.0, 0.0);
	else if(key == 'A')cameraTranslate(-1.0, 0.0);
	else if(key == 'Q');
	else if(key == 'E');
}

void Renderer::cameraRotate(int parallel, int vertical){
	glm::vec3 view = camera_lookat_position - camera_position;
	//view = glm::normalize(view);
	glm::vec3 dir_parallel = glm::cross(view, camera_up);
	glm::vec3 dir_vertical = glm::cross(view, dir_parallel);
	printf("rotate  parallel:%d  vertical:%d  normalized_view.y=%f\n"
			,parallel, vertical,glm::normalize(view).y);
	if(fabs(glm::normalize(view).y)>0.9 && vertical>0)vertical=0;
	view = glm::rotate(view,(float)(0.2f*vertical),-dir_parallel);
	view = glm::rotate(view,(float)(0.2f*parallel),dir_vertical);
	printf("view : (%f, %f, %f)\n",view.x, view.y, view.z);
	camera_lookat_position = camera_position + view;
	findLookAtMatrix();
	//view_matrix = glm::lookAt(camera_position,camera_lookat_position,camera_up);
	//view_matrix = glm::rotate(view_matrix,10.0f,glm::vec3(0.0f, 1.0f, 0.0f));
	pv_matrix = projection_matrix * view_matrix;

	view_changed = true;
}

void Renderer::DrawTriangle(std::vector<float> &pixel_position, int mode){
	//return;
	int w,h,target_w,target_h,pre_w,pre_h,w_offset,h_offset,count,color=0;
	int rgb[3];
	float slope,t,target_z,pre_z;
	//printf("pixel_position.size = %d\n",pixel_position.size());
	for(unsigned int i=0; i<pixel_position.size(); i+=3){

		if(i%9==0){
			//t = 3/(pixel_position[i+2] + pixel_position[i+5] + pixel_position[i+8]);
			//color = (color+1)%3;
			//memset(rgb, 0, sizeof(int)*3);
			//rgb[color] = 100 + mode*40;
			rgb[0] = (i*7)%120 + mode*40 +50;
			rgb[1] = (i*11)%120 + mode*40 +50;
			rgb[2] = (i*17)%120 + mode*40 +50;
			//printf("i=%u\n",i);
			//std::swap(pixel_position[i+1], pixel_position[i+3]);
			//std::swap(pixel_position[i], pixel_position[i+2]);
			if(pixel_position[i+1] < pixel_position[i+4]){
				std::iter_swap(pixel_position.begin() + i+1, pixel_position.begin() + i+4);
				std::iter_swap(pixel_position.begin() + i, pixel_position.begin() + i+3);
				std::iter_swap(pixel_position.begin() + i+2, pixel_position.begin() + i+5);
			}
			if(pixel_position[i+1] < pixel_position[i+7]){
				//std::swap(pixel_position[i+1], pixel_position[i+5]);
				//std::swap(pixel_position[i], pixel_position[i+4]);
				std::iter_swap(pixel_position.begin() + i+1, pixel_position.begin() + i+7);
				std::iter_swap(pixel_position.begin() + i, pixel_position.begin() + i+6);
				std::iter_swap(pixel_position.begin() + i+2, pixel_position.begin() + i+8);
			}
			if(pixel_position[i+4] > pixel_position[i+7]){
				//std::swap(pixel_position[i+3], pixel_position[i+5]);
				//std::swap(pixel_position[i+2], pixel_position[i+4]);
				std::iter_swap(pixel_position.begin() + i+4, pixel_position.begin() + i+7);
				std::iter_swap(pixel_position.begin() + i+3, pixel_position.begin() + i+6);
				std::iter_swap(pixel_position.begin() + i+5, pixel_position.begin() + i+8);
			}
			//printf("y1:%f  y2:%f  y3:%f\n",pixel_position[i+1],pixel_position[i+4],pixel_position[i+7]);
		}
		//fillTriangle(pixel_position,i);
		//return;
		//continue;

		target_w = (int)pixel_position[i];
		target_h = (int)pixel_position[i+1];
		target_z = pixel_position[i+2];

		//screen_pixel_data[(h*window_width + w)*3] =pixel_color;

		if(i%9!=0){
			pre_w = (int)pixel_position[i-3];
			pre_h = (int)pixel_position[i-2];
			pre_z = pixel_position[i-1];

			w_offset = target_w - pre_w;
			h_offset = target_h - pre_h;

			if(w_offset!=0.0)slope = (float)(h_offset)/(w_offset);
			else slope=100000;
			//slope = (float)(h_offset)/(w_offset);
			if(slope*slope < 1){
				count=1;	if(w_offset<0)count=-1;
				for(int j=0; j!=w_offset+count; j+=count){
					h = (int)(pre_h + slope*j);
					w = pre_w+j;
					t = pre_z + ((target_z- pre_z)/(w_offset))*j ;
					//printf("pre_z:%f target_z:%f z=%f\n",pre_z,target_z,t);
					//t = 1/t;
					if(i%9==3){
						if(h>=0 && h<1500){
							triangle_bound[h][0] = w;
							bound_depth[h][0] = t;
						}
					}
					else{
						if(h>=0 && h<1500){
							triangle_bound[h][1] = w;
							bound_depth[h][1] = t;
						}
					}
					if(h >= window_height || h<0 || w<0 || w>=window_width)continue;
					//printf("h=%d  w=%d\n",h,w);
					//screen_pixel_data[(h*window_width + w)*3+color] = 100 + mode*40;
					DrawPixel(w, h, 1/t, rgb[0], rgb[1], rgb[2]);
				}
			}
			else{
				count=1;	if(h_offset<0)count=-1;
				for(int j=0; j!=h_offset+count; j+=count){
					h = (int)(pre_h + j);
					w = (int)(pre_w+j/slope);
					t = pre_z + ((target_z- pre_z)/(h_offset))*j ;
					//t = 1/t;
					if(i%9==3){
						if(h>=0 && h<1500){
							triangle_bound[h][0] = w;
							bound_depth[h][0] = t;
						}
					}
					else{
						if(h>=0 && h<1500){
							triangle_bound[h][1] = w;
							bound_depth[h][1] = t;
						}
					}
					if(h >= window_height || h<0 || w<0 || w>=window_width)continue;
					//printf("h=%d  w=%d\n",h,w);
					//screen_pixel_data[(h*window_width + w)*3+color] = 100 + mode*40;
					DrawPixel(w, h, 1/t, rgb[0], rgb[1], rgb[2]);
				}
			}
		}

		if(i%9==6){
			pre_w = (int)pixel_position[i-6];
			pre_h = (int)pixel_position[i-5];
			pre_z = pixel_position[i-4];

			w_offset = target_w - pre_w;
			h_offset = target_h - pre_h;

			if (w_offset != 0.0) slope = (float) (h_offset) / (w_offset);
			else slope = 100000;
			//slope = (float)(h_offset)/(w_offset);
			if(slope*slope < 1){
				count=1;	if(w_offset<0)count=-1;
				for(int j=0; j!=w_offset+count; j+=count){
					h = (int)(pre_h + slope*j);
					w = pre_w+j;
					t = pre_z + ((target_z- pre_z)/(w_offset))*j ;
					if(h>=0 && h<1500){
						triangle_bound[h][1] = w;
						bound_depth[h][1] = t;
					}
					if(h >= window_height || h<0 || w<0 || w>=window_width)continue;
					//printf("h=%d  w=%d\n",h,w);
					//screen_pixel_data[(h * window_width + w) * 3 + color] =100 + mode * 40;
					DrawPixel(w, h, 1/t, rgb[0], rgb[1], rgb[2]);
				}
			}
			else{
				count=1;	if(h_offset<0)count=-1;
				for(int j=0; j!=h_offset+count; j+=count){
					h = (int)(pre_h + j);
					w = (int)(pre_w+j/slope);
					t = pre_z + ((target_z- pre_z)/(h_offset))*j ;
					//t = 1/t;
					if(h>=0 && h<1500){
						triangle_bound[h][1] = w;
						bound_depth[h][1] = t;
					}
					if(h >= window_height || h<0 || w<0 || w>=window_width)continue;
					//printf("h=%d  w=%d\n",h,w);
					//screen_pixel_data[(h*window_width + w)*3+color] = 100 + mode*40;
					DrawPixel(w, h, 1/t, rgb[0], rgb[1], rgb[2]);
				}
			}
			//continue;
			pre_h = (int)pixel_position[i-5]-1; if(pre_h >= window_height)pre_h = window_height-1;
			target_h = (int)pixel_position[i-2]; if(target_h <0)target_h = 0;
			for(int j = pre_h; j>target_h; j--){
				if(triangle_bound[j][0] > triangle_bound[j][1]){
					std::swap(triangle_bound[j][0],triangle_bound[j][1]);
					std::swap(bound_depth[j][0],bound_depth[j][1]);
				}
				w_offset = triangle_bound[j][1] - triangle_bound[j][0];
				for(int k = triangle_bound[j][0]; k<triangle_bound[j][1]; k++ ){
					if(j >= window_height || j<0 || k<0 || k>=window_width)continue;
					if(w_offset != 0.0)t = bound_depth[j][0] + (bound_depth[j][1] - bound_depth[j][0])/w_offset*(k-triangle_bound[j][0]);
					else t=bound_depth[j][0];
					//printf("pre_z:%f target_z:%f z=%f\n",bound_depth[j][0],bound_depth[j][1],t);
					//screen_pixel_data[(j*window_width + k)*3+color] = 100 + mode*40;
					DrawPixel(k, j, 1/t, rgb[0], rgb[1], rgb[2]);
				}
			}
		}

	}

}

void Renderer::DrawLine(std::vector<float> &pixel_position, int mode){
	int w,h,target_w,target_h,pre_w,pre_h,w_offset,h_offset,count,color=0;
	float slope,tmp;
	//printf("pixel_position.size = %d\n",pixel_position.size());
	for(unsigned int i=0; i<pixel_position.size(); i+=3){

		target_w = (int)pixel_position[i];
		target_h = (int)pixel_position[i+1];

		//screen_pixel_data[(h*window_width + w)*3] =pixel_color;

		if(i%6==3){
			pre_w = (int)pixel_position[i-3];
			pre_h = (int)pixel_position[i-2];

			w_offset = target_w - pre_w;
			h_offset = target_h - pre_h;

			//if(w_offset!=0)slope = (float)(h_offset)/(w_offset);
			//else slope=0;
			slope = (float)(h_offset)/(w_offset);
			if(slope*slope < 1){
				count=1;	if(w_offset<0)count=-1;
				for(int j=0; j!=w_offset+count; j+=count){
					h = (int)(pre_h + slope*j);
					w = pre_w+j;
					if(i%9==3){
						triangle_bound[h][0] = w;
					}
					else{
						triangle_bound[h][1] = w;
					}
					if(h >= window_height || h<0 || w<0 || w>=window_width)continue;
					//printf("h=%d  w=%d\n",h,w);
					screen_pixel_data[(h*window_width + w)*3] = 100 + mode*40;//255.0f/w_offset*j;
					screen_pixel_data[(h*window_width + w)*3+1] = 100 + mode*40;
				}
			}
			else{
				count=1;	if(h_offset<0)count=-1;
				for(int j=0; j!=h_offset+count; j+=count){
					h = (int)(pre_h + j);
					w = (int)(pre_w+j/slope);
					if (i % 9 == 3) {
						triangle_bound[h][0] = w;
					} else {
						triangle_bound[h][1] = w;
					}
					if(h >= window_height || h<0 || w<0 || w>=window_width)continue;
					//printf("h=%d  w=%d\n",h,w);
					screen_pixel_data[(h*window_width + w)*3] = 100 + mode*40;//255.0f/h_offset*j;
					screen_pixel_data[(h*window_width + w)*3+1] = 100 + mode*40;
				}
			}
		}

	}

}

void Renderer::DrawPixel(int x, int y, float t, int color_r, int color_g, int color_b){
	//printf("t=%f\n",t);
	//if(t<0) return;
	if(z_buffer[(y*window_width + x)]==0 || t < (float)z_buffer[(y*window_width + x)]){

		screen_pixel_data[(y*window_width + x)*3] = color_r;
		screen_pixel_data[(y*window_width + x)*3+1] = color_g;
		screen_pixel_data[(y*window_width + x)*3+2] = color_b;

		z_buffer[(y*window_width + x)] = t;
	}
}

void Renderer::findLookAtMatrix(){
	//view_matrix = glm::lookAt(camera_position,camera_lookat_position,camera_up);
	//return;
	glm::vec3 zaxis = glm::normalize(camera_lookat_position - camera_position);
	glm::vec3 xaxis = glm::normalize(glm::cross(camera_up, zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);
	/*view_matrix[0] = glm::vec4(xaxis, -glm::dot(xaxis, camera_position));
	view_matrix[1] = glm::vec4(yaxis, -glm::dot(yaxis, camera_position));
	view_matrix[2] = glm::vec4(zaxis, -glm::dot(zaxis, camera_position));
	view_matrix[3] = glm::vec4(0, 0, 0, 1);*/

	view_matrix[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0);
	view_matrix[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0);
	view_matrix[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0);
	view_matrix[3] = glm::vec4(-glm::dot(xaxis, camera_position), -glm::dot(yaxis, camera_position), -glm::dot(zaxis, camera_position), 1);

}
