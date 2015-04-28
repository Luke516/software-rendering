/*
 * UI.h
 *
 *  Created on: 2014/10/18
 *      Author: LukeWu
 */

#ifndef UI_H_
#define UI_H_

#include <vector>
#include "MyWindow.h"
#include "Renderer.h"

class UI {
public:
	UI();
	static void setWindow(MyWindow *w){window = w;}
	static void translateVertexTo3DSpace(std::vector<float> *vertex_list);
protected:
	static MyWindow *window;
};

#endif /* UI_H_ */
