/*
 * IOhandler.h
 *
 *  Created on: 2014/8/19
 *      Author: LukeWu
 */

#ifndef IOHANDLER_H_
#define IOHANDLER_H_
#include <vector>
#include "Renderer.h"
#include "Keyboard.h"
#include "MyWindow.h"

class BasicObject;

class IOhandler {
public:
	IOhandler(MyWindow *w);
	void clear();
	void init();
	void push(BasicObject *np);
	void render();
	void update();
	void resize(){renderer.resize();}
	void keyActive(int key, int action);
	void mouseActive(int button, double x_pos, double y_pos);
private:
	std::vector<BasicObject *>object_list;
	KeyBoard keyboard;
	Renderer renderer;
	MyWindow *window_ptr;
};

#endif /* IOHANDLER_H_ */
