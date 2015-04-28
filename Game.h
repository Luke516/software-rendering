/*
 * Game.h
 *
 *  Created on: 2014/7/30
 *      Author: LukeWu
 */

#ifndef GAME_H_
#define GAME_H_
#include <GLFW/glfw3.h>
#include <ctime>
#include "IOhandler.h"
#include "MyWindow.h"

enum enum_state{
	state_none,state_menu,state_game,state_setting,state_end
};

class Game {
public:
	Game(int window_width=960, int window_height=720);
	void init();
	void changeNextState(enum_state next);
	void changeState();
	void gameActive();
	void initState();
	void keyActive(int key, int action);
	void mouseActive(int button, double x_pos, double y_pos);
	void windowResize(int width, int height);
	GLFWwindow* getWindow(){return window.getWindow();}
private:
	clock_t loop_start_time,loop_end_time,loop_elapsed_time;
	bool terminate;
	enum_state state;
	enum_state next_state;
	MyWindow window;
	IOhandler io_handler;
	std::vector<BasicObject *>object_list;
};

#endif /* GAME_H_ */
