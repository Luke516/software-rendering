/*
 * AllOption.h
 *
 *  Created on: 2015¦~3¤ë26¤é
 *      Author: LukeWu
 */

#ifndef ALLOPTION_H_
#define ALLOPTION_H_

#include "Label.h"

class RotateOption : public Label{
public:
	RotateOption(float _x, float _y, std::string _text, glm::vec3 v);
	virtual ~RotateOption();
	virtual float mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos);
	virtual void keyActive(int key, int action);
	virtual void update();
	virtual void setMode(int _mode);
private:
	bool active_flag;
	bool direction;
	glm::vec3 rotate_vector;
};

class TranslateOption : public Label{
public:
	TranslateOption(float _x, float _y, std::string _text, glm::vec3 v);
	virtual ~TranslateOption();
	virtual float mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos);
	virtual void keyActive(int key, int action);
	virtual void update();
	virtual void setMode(int _mode);
private:
	bool active_flag;
	bool direction;
	glm::vec3 translate_vector;
};

class ScaleOption : public Label{
public:
	ScaleOption(float _x, float _y, std::string _text, glm::vec3 v);
	virtual ~ScaleOption();
	virtual float mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos);
	virtual void keyActive(int key, int action);
	virtual void update();
	virtual void setMode(int _mode);
private:
	bool active_flag;
	bool direction;
	glm::vec3 scale_vector;
};

class MirrorOption : public Label{
public:
	MirrorOption(float _x, float _y, std::string _text, glm::vec3 v);
	virtual ~MirrorOption();
	virtual float mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos);
	virtual void keyActive(int key, int action);
	virtual void update();
	virtual void setMode(int _mode);
private:
	bool active_flag;
	bool direction;
	glm::vec3 mirror_vector;
};

class ShearOption : public Label{
public:
	ShearOption(float _x, float _y, std::string _text, glm::vec3 v);
	virtual ~ShearOption();
	virtual float mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos, int y_pos);
	virtual void keyActive(int key, int action);
	virtual void update();
	virtual void setMode(int _mode);
private:
	bool active_flag;
	bool direction;
	glm::vec3 shear_vector;
};

#endif /* ALLOPTION_H_ */
