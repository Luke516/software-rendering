/*
 * LightSource.cpp
 *
 *  Created on: 2015¦~4¤ë27¤é
 *      Author: LukeWu
 */

#include "LightSource.h"

LightSource::LightSource(float x_, float y_, float z_, float r_, float g_, float b_):
	light(r_,g_,b_,0.0,x_,y_,z_,1.0)
{

}

LightSource::~LightSource() {
}

void LightSource::render(Renderer* renderer, bool view_changed) {

}

void LightSource::keyActive(int key, int action) {
}
