/*
 * Light.h
 *
 *  Created on: 2015¦~4¤ë27¤é
 *      Author: LukeWu
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light {
public:
	Light(float r, float g, float b, float a, float x, float y, float z, float p);
	glm::vec4& getRGBA(){return rgba;}
	glm::vec3& getPosition(){return position;}
	float getPower(unsigned int index){return power;}
protected:
	glm::vec4 rgba;
	glm::vec3 position;
	float power;
};

#endif /* LIGHT_H_ */
