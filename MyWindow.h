/*
 * MyWindow.h
 *
 *  Created on: 2014/8/5
 *      Author: LukeWu
 */

#ifndef MYWINDOW_H_
#define MYWINDOW_H_
#include <GLFW/glfw3.h>

class MyWindow {
public:
	MyWindow(int width,int height);
	bool shouldClose(){return glfwWindowShouldClose(window);}
	void swapBuffer(){glfwSwapBuffers(window);}
	////////////////////////////////////////////////////////////////////////////////////////////
	int getWindowHeight(){return h;}                                  //  QWQ
	int getWindowWidth(){return w;}                                   //	  QAQ
	void setWindowHeight(int hh){h = hh;}                             //		  Q_Q
	void setWindowWidth(int ww){w = ww;}                              //			  0'_'0
	////////////////////////////////////////////////////////////////////////////////////////////
	float getWindowHeight3D(){return h_3D;}
	float getWindowWidth3D(){return w_3D;}
	void setWindowHeight3D(float hh_3D){h_3D = hh_3D;}
	void setWindowWidth3D(float ww_3D){w_3D = ww_3D;}
	////////////////////////////////////////////////////////////////////
	GLFWwindow* getWindow(){return window;}
private:
	GLFWwindow* window;
	int h,w;
	float h_3D, w_3D;
};

#endif /* MYWINDOW_H_ */
