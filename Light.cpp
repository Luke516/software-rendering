/*
 * Light.cpp
 *
 *  Created on: 2015¦~4¤ë27¤é
 *      Author: LukeWu
 */

#include "Light.h"

Light::Light(float r, float g, float b, float a, float x, float y, float z, float p)
	:type(0), on(true), rgba(glm::vec4(r,g,b,a)), position(glm::vec3(x,y,z)), power(p)
{
	// TODO Auto-generated constructor stub

}

