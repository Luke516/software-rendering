/*
 * MyWindow.cpp
 *
 *  Created on: 2014/8/5
 *      Author: LukeWu
 */

#include <cstdio>
#include "MyWindow.h"

MyWindow::MyWindow(int width, int height):w(width),h(height)
{
	window = glfwCreateWindow(width, height, "CGHW2", NULL,NULL);
	if (!window) {
		printf("ERROR: could not open window with GLFW3\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
}

