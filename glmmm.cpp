/*
 * glmmm.cpp
 *
 *  Created on: 2015¦~3¤ë31¤é
 *      Author: LukeWu
 */

#include "glmmm.h"

namespace glmmm {

	glm::mat4 rotate(glm::mat4 matrix, float angle, glm::vec3 axis){
		return matrix;
	}

	glm::mat4 translate(glm::mat4 matrix, glm::vec3 translate_vector){
		glm::mat4 translate_matrix(1.0);
		translate_matrix[3].x = translate_vector.x;
		translate_matrix[3].y = translate_vector.y;
		translate_matrix[3].z = translate_vector.z;
		//return translate_matrix;
		return matrix * translate_matrix;
		//return glm::translate(matrix, translate_vector);
	}

	float interpolationf(float a, float b, float p){
		float ans;
		if(p>1)return a;
		ans = a*p + b*(1-p);
		return ans;
	}
} /* namespace glmmm */
