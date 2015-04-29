/*
 * glmmm.h
 *
 *  Created on: 2015¦~3¤ë31¤é
 *      Author: LukeWu
 */

#ifndef GLMMM_H_
#define GLMMM_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace glmmm {

	glm::mat4 rotate(glm::mat4 matrix, float angle, glm::vec3 axis);
	glm::mat4 translate(glm::mat4 matrix, glm::vec3 translate_vector);
	float interpolationf(float, float, float);

} /* namespace glmmm */

#endif /* GLMMM_H_ */
