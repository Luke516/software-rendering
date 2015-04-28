/*
 * main.cpp
 *
 *  Created on: 2015¦~3¤ë12¤é
 *      Author: LukeWu
 */

#define GLM_FORCE_RADIANS

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <cstdio>
#include <cmath>
#include <vector>
#include "MyWindow.h"
#include "MyShader.h"
#include "ObjLoader.h"
#include "Renderer.h"
#include "KeyBoard.h"
#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game* game_ptr;
bool mouse_right_button_pressed = false;
bool mouse_middle_button_pressed = false;
bool mouse_left_button_pressed = false;

int	mouse_x,mouse_y;

void  myKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
 {
	game_ptr->keyActive(key, action);

}

void myCourserPositionCallback(GLFWwindow *window, double x, double y){
	printf("x = %lf  y = %lf\n",x,y);
	game_ptr->mouseActive(0,x,y);
	if(mouse_middle_button_pressed){
		//printf("rotate!!\n");
		game_ptr ->mouseActive(3,x-mouse_x,y-mouse_y);
	}
	else if(mouse_left_button_pressed){
		game_ptr ->mouseActive(1,x-mouse_x,y-mouse_y);
	}
	mouse_x=x;
	mouse_y=y;
}

void myMouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		mouse_left_button_pressed = true;
		game_ptr->mouseActive(1,0,0);
	}
	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		mouse_left_button_pressed = false;
		//game_ptr->mouseActive(-1,0,0);
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
		mouse_right_button_pressed = true;
		game_ptr->mouseActive(2,0,0);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		mouse_right_button_pressed = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
			mouse_middle_button_pressed = true;
		}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
			mouse_middle_button_pressed = false;
		}
}

void myScrollCallback(GLFWwindow *window, double xoffset, double yoffset){
	printf("scroll-- x:%lf y:%lf\n",xoffset,yoffset);
	if(yoffset > 0){
		game_ptr->mouseActive(4,0,0);
	}
	else if(yoffset < 0){
		game_ptr->mouseActive(-4,0,0);
	}
}

void myWindowResizeCallback(GLFWwindow *window, int width, int hieght){

	glfwSetWindowSize(window, 960, 720);

	/*if(width<640 || hieght<480){
		width=640; hieght=480;

	}
	game_ptr -> windowResize(width, hieght);*/
}



int main() {
	/* test
	HexCoordinate a(1,2),b(5,9),c,d;
	c = a+b;
	d = a+c;
	if(a.getX()+b.getX()==c.getX() && a.getY()+b.getY()==c.getY())printf("WOWOWOWOW!!!\n");
	*/

	if (!glfwInit()) {
		printf("ERROR: could not start GLFW3\n");
		return 1;
	}

	Game game;
	game_ptr = &game;

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	////////////////////////////////////////////////////////////////////////////////////////////

	glfwSetKeyCallback(game.getWindow(),myKeyCallback);
	glfwSetCursorPosCallback(game.getWindow(),myCourserPositionCallback);
	glfwSetMouseButtonCallback(game.getWindow(),myMouseButtonCallback);
	glfwSetWindowSizeCallback(game.getWindow(), myWindowResizeCallback);
	glfwSetScrollCallback(game.getWindow(), myScrollCallback);

	game.init();
	game.gameActive();


	glfwTerminate();
	return 0;
}


/*
mouseActive:button:
0->no button
1->left button pressed       -1->left button released
2->right button pressed		 -2->right button released
3->middle button pressed     -3->middle button released
4->scroll up
5->scroll down
*/

/*

#define GLM_FORCE_RADIANS

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <cstdio>
#include <cmath>
#include <vector>
#include "MyShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void pointToLine(std::vector<float> &vertices);


int main() {

	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		printf("ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "test", NULL,
	NULL);
	if (!window) {
		printf("ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);



	////////////////////////////////////////////////////////////////////////////////////////////

	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	glm::mat4 View = glm::lookAt(glm::vec3(2, 2, 4), glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0));
	glm::mat4 Model = glm::mat4(1.0f);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	//glEnable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);

	GLuint vertex_shader = MyShader::CreateShaderFromFile("VertexShader.vert",
	GL_VERTEX_SHADER);
	GLuint fragment_shader = MyShader::CreateShaderFromFile("FragmentShader.frag",
	GL_FRAGMENT_SHADER);

	unsigned int shader_programme = glCreateProgram();
	glAttachShader(shader_programme, vertex_shader);
	glAttachShader(shader_programme, fragment_shader);
	glLinkProgram(shader_programme);

	float points[]={
	 0.5, 0.5, 0.1,
	 0.9, 0.0, 0.1,
	 0.0, 0.9 ,0.1
	 };
	std::vector<float> vertices(points, points+9);
	pointToLine(vertices);

	 GLfloat colors[] = {
	 0.583f,  0.771f,  0.014f,
	 0.609f,  0.115f,  0.436f,
	 0.327f,  0.483f,  0.844f
	 };

	GLuint vbo_points = 0;
	glGenBuffers(1, &vbo_points);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_points);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint vbo_colors=0;
	 glGenBuffers(1,&vbo_colors);
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
	 glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), colors, GL_STATIC_DRAW);

	 glEnableVertexAttribArray(1);
	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint matrix_location = glGetUniformLocation(shader_programme, "MVP");

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);

		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &MVP[0][0]);

		glDrawArrays(GL_POINTS, 0, 3);

		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return 0;
}

std::vector<float> projectTo2d(std::vector<float> vertices){
	std::vector<float> projected_vertices;
	return projected_vertices;
}

void pointToLine(std::vector<float> &vertices){
	std::vector<float> lines;
	float dis; //distance between two points
	int segement;
	for(int i=0; i<vertices.size(); i+=9){

		//dis = sqrt(glm::abs(vertices[i]-vertices[i+3])^2 + glm::abs(vertices[i+1]-vertices[i+4])^2 + glm::abs(vertices[i+2]-vertices[i+5])^2);
	}
	return;
}

void drawLine(){

}*/
