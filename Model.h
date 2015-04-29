/*
 * Model.h
 *
 *  Created on: 2015¦~3¤ë16¤é
 *      Author: LukeWu
 */

#ifndef MODEL_H_
#define MODEL_H_
#include <string>
#include "SelectableObject.h"
#include "Renderer.h"

class Model : public SelectableObject{
public:
	//Ship();
	Model(float _x, float _y, float _z, std::string file_name);
	virtual ~Model();
	virtual void render(Renderer* renderer, bool view_changed);
	virtual void update();
	virtual void keyActive(int key, int action);
	virtual void setMode(int next_mode);
	void renderOtho(Renderer* renderer);
	void swapProjection(){perspective = !perspective;}
	void rotate(float angle, glm::vec3 axis, glm::vec3 offset=glm::vec3(0,0,0));
	void translate(glm::vec3 translate_vector);
	void scale(glm::vec3 scale_vector);
	void mirror(glm::vec3 mirror_vector);
	void shear(glm::vec3 shear_vector);
	void setRotateCenter(glm::vec3 _center){rotate_center = glm::vec3(_center);}
private:
	glm::mat4 rotate_matrix;
	bool visible;
	bool perspective;
	std::vector<float> projected_vertices;
	std::vector<float> vertex_color;
	glm::vec3 rotate_center;
};

#endif /* MODEL_H_ */
