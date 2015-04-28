/*
 * Game.cpp
 *
 *  Created on: 2014/7/30
 *      Author: LukeWu
 */

#include <cstdio>
#include "Game.h"
#include "BasicObject.h"
#include "Field.h"
#include "UI.h"

Game::Game(int window_width, int window_height)
	:loop_start_time(0),loop_end_time(0),loop_elapsed_time(10),terminate(false),
	 state(state_none),next_state(state_game),window(window_width,window_height),io_handler(&window)
{
	UI::setWindow(&window);
}

void Game::init(){
	io_handler.init();
}

void Game::gameActive(){
	while(!terminate && !window.shouldClose()){
		if(next_state != state_none){
			changeState();
		}
		loop_start_time = clock();
		io_handler.update();
		io_handler.render();
		loop_end_time = clock();
		while(loop_end_time - loop_start_time < loop_elapsed_time){
			loop_end_time = clock();
		}
	}
	return;
}

void Game::changeState(){
	state = next_state;
	next_state = state_none;
	initState();
}

void Game::initState(){
	switch(state){
	case state_game:
	{
		Field* np = new Field;
		io_handler.push(np);
		break;
	}
	default:
		break;
	}
	return;
}

void Game::keyActive(int key, int action){
	io_handler.keyActive(key, action);
}
void Game::mouseActive(int button, double x_pos, double y_pos){
	io_handler.mouseActive(button, x_pos, y_pos);
}

void Game::windowResize(int width, int height){
	printf("resize~ %d %d\n",width,height);
	window.setWindowWidth(width);
	window.setWindowHeight(height);
	io_handler.resize();
}
