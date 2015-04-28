/*
 * UI.cpp
 *
 *  Created on: 2014/10/18
 *      Author: LukeWu
 */

#include <cstdio>
#include "UI.h"

MyWindow *UI::window;

UI::UI() {

}

void UI::translateVertexTo3DSpace(std::vector<float> *vertex_list){
	float h = (float)(window -> getWindowHeight());
	float w = (float)(window -> getWindowWidth());
	float h_3d = (float)(window -> getWindowHeight3D());
	float w_3d = (float)(window -> getWindowWidth3D());
	printf("h:%f  w:%f  h3d:%f  w3d:%f\n",h,w,h_3d,w_3d);
	for(unsigned int i=0; i<vertex_list->size(); i+=3){
		(*vertex_list)[i] = ((*vertex_list)[i]/w-0.5)*w_3d;
		(*vertex_list)[i+1] = -((*vertex_list)[i+1]/h-0.5)*h_3d;
		(*vertex_list)[i+2] = -1;
	}
	return;
}
