/*
 * Field.h
 *
 *  Created on: 2014/7/30
 *      Author: LukeWu
 */

#ifndef FIELD_H_
#define FIELD_H_
#include <vector>
#include "Model.h"
#include "Label.h"
#include "CommandLine.h"
#include "Renderer.h"
#include "BasicObject.h"

class Field:public BasicObject {
public:
	Field();
	virtual ~Field();
	virtual void render(Renderer *renderer, bool view_changed);
	virtual void update();
	virtual void keyActive(int key, int action);
	virtual void mouseActive(int button, glm::vec3 pos, glm::vec3 dir,int x_pos,int y_pos);
	void executeCommand(std::string command);
private:
	//FleetPanel fleet_panel;
	//HexMap map;
	//PathFinder path_finder;
	//std::vector<Ship *> ship_list;
	//std::vector<Planet *> planet_list;
	std::vector<Model *> model_list;
	std::vector<Label *> label_list;
	std::vector<SelectableObject *> selectable_list;
	std::vector<unsigned int> touched_list;
	std::vector<unsigned int> selected_list;
	CommandLine command;

	//TurnCounter turn_counter;
};

#endif /* FIELD_H_ */
