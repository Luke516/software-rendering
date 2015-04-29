/*
 * Field.cpp
 *
 *  Created on: 2014/7/30
 *      Author: LukeWu
 */

#include <GL/glew.h>
#include <cstdio>
#include <algorithm>
#include "Field.h"
#include "AllOption.h"

Field::Field():command(50,640,"")
{
	Model *model = new Model(0,10,0,"ship00.obj");
	Model *model2 = new Model(10,10,0,"ship00.obj");
	Model *model3 = new Model(-10,10,0,"ship00.obj");
	Label *label = new RotateOption(0,10,"rotate x",glm::vec3(1,0,0));
	Label *label2 = new RotateOption(0,60,"rotate y",glm::vec3(0,1,0));
	Label *label3 = new RotateOption(0,110,"rotate z",glm::vec3(0,0,1));
	Label *label4 = new TranslateOption(0,160,"translate x",glm::vec3(1,0,0));
	Label *label5 = new TranslateOption(0,210,"translate y",glm::vec3(0,1,0));
	Label *label6 = new TranslateOption(0,260,"translate z",glm::vec3(0,0,1));
	Label *label7 = new ScaleOption(0,310,"scale x",glm::vec3(1.2,1,1));
	Label *label8 = new ScaleOption(0,360,"scale y",glm::vec3(1,1.2,1));
	Label *label9 = new ScaleOption(0,410,"scale z",glm::vec3(1,1,1.2));
	Label *label10 = new MirrorOption(0,460,"mirror x",glm::vec3(1,0,0));
	Label *label11 = new MirrorOption(0,510,"mirror y",glm::vec3(0,1,0));
	Label *label12 = new MirrorOption(0,560,"mirror z",glm::vec3(0,0,1));
	Label *label13 = new ShearOption(0,610,"shear",glm::vec3(1,0,0));
	model_list.push_back(model);
	selectable_list.push_back(model);
	model_list.push_back(model2);
	selectable_list.push_back(model2);
	model_list.push_back(model3);
	selectable_list.push_back(model3);
	label_list.push_back(label);
	selectable_list.push_back(label);
	label_list.push_back(label2);
	selectable_list.push_back(label2);
	label_list.push_back(label3);
	selectable_list.push_back(label3);
	label_list.push_back(label4);
	selectable_list.push_back(label4);
	label_list.push_back(label5);
	selectable_list.push_back(label5);
	label_list.push_back(label6);
	selectable_list.push_back(label6);
	label_list.push_back(label7);
	selectable_list.push_back(label7);
	label_list.push_back(label8);
	selectable_list.push_back(label8);
	label_list.push_back(label9);
	selectable_list.push_back(label9);
	label_list.push_back(label10);
	selectable_list.push_back(label10);
	label_list.push_back(label11);
	selectable_list.push_back(label11);
	label_list.push_back(label12);
	selectable_list.push_back(label12);
	label_list.push_back(label13);
	selectable_list.push_back(label13);
	selectable_list.push_back(&command);

	Label::setTarget(model);

	Light light(0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 50.0, 10.0);
	light_list.push_back(light);
}

Field::~Field(){
	model_list.clear();
}

void Field::render(Renderer *renderer, bool view_changed){

	renderer->setRenderLightList(light_list);

	for(unsigned int i=0; i<model_list.size(); i++){
		//printf("i = %u\n",i);
		model_list[i]->render(renderer, view_changed);
	}
	for(unsigned int i=0; i<label_list.size(); i++){
		//printf("i = %u\n",i);
		label_list[i]->render(renderer, view_changed);
	}
	command.render(renderer, view_changed);
	return;
}

void Field::update(){
	for (unsigned int i = 0; i < model_list.size(); i++) {
		model_list[i]->update();
	}
	for(unsigned int i=0; i<label_list.size(); i++){
		label_list[i]->update();
	}
	return;
}

void Field::keyActive(int key, int action){
	if(command.getActive() && action == GLFW_PRESS){
		if(key == GLFW_KEY_ENTER){
			executeCommand(command.getCommand());
		}
		command.keyActive(key, action);
		return;
	}
	if(key == 'N' && action == GLFW_PRESS ){

	}
	if(key == 'M' && action == GLFW_PRESS ){
		for(unsigned int i=0; i<label_list.size(); i++){
			label_list[i]->swapVisible();
		}
	}
	if(key == 'P' && action == GLFW_PRESS ){
		for(unsigned int i=0; i<model_list.size(); i++){
			model_list[i]->swapProjection();
		}
	}
	for(unsigned int i=0; i<selectable_list.size(); i++){
		selectable_list[i]->keyActive(key, action);
	}
	return;
}

void Field::mouseActive(int button, glm::vec3 pos, glm::vec3 dir, int x_pos, int y_pos){

	if (button == 1) {//click an object
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			selectable_list[selected_list[i]] -> setMode(0);//clear previous selected
		}
		selected_list.clear();

		for (unsigned int i = 0; i < touched_list.size(); i++) {
			selectable_list[touched_list[i]] -> setMode(2);//select object cursor touched
			selected_list.push_back(touched_list[i]);
		}

		return;
	}



	for(unsigned int i=0; i<touched_list.size(); i++){
		if(selectable_list[touched_list[i]] -> getMode() == 1)
			selectable_list[touched_list[i]] -> setMode(0);
	}

	touched_list.clear();


	float t(-1),tmin(-1.0);
	unsigned int selected_object = 0;

	if(dir[0]==0)dir[0]=0.00001;
	if(dir[1]==0)dir[1]=0.00001;
	if(dir[2]==0)dir[2]=0.00001;

	for(unsigned int i=0; i<selectable_list.size(); i++){
		//printf("selectable size : %d\n",selectable_list.size());
		t = selectable_list[i] -> mouseActive(button, pos, dir,x_pos,y_pos);
		if((tmin<0 || t<tmin) && t>0){
			selected_object = i+1;
			tmin = t;
		}
	}

	printf("selected object : %d\n",selected_object);

	if(selected_object>0){
		if(selectable_list[selected_object-1] -> getMode() == 0){
			selectable_list[selected_object-1] -> setMode(1);
			touched_list.push_back(selected_object-1);
		}
	}
	return;
}

void Field::executeCommand(std::string command){
	printf("execute : %s\n",command.c_str());
	const char *s = command.c_str();
	if(s[0] == 'l'){
		std::string path(s+1);
		Model *new_model = new Model(0,10,0,(path+".obj").c_str());
		model_list.push_back(new_model);
		selectable_list.push_back(new_model);
	}
	if(s[0] == 'd'){
		unsigned int no;
		sscanf(s+1,"%u",&no);
		delete model_list[no];
		model_list.erase(model_list.begin()+no);
		selectable_list.erase(selectable_list.begin()+no);
	}
	if(s[0] == 'r'){
		int no,_x, _y, _z;
		sscanf(s+1,"%d %d %d %d",&no ,&_x, &_y, &_z);
		model_list[no]->setRotateCenter(glm::vec3(_x, _y, _z));
	}
	if(s[0] == 's'){
		int no;
		std::vector<SelectableObject *>::iterator it;
		sscanf(s+1,"%d",&no);
		Model *target = model_list[no];
		it = std::find(selectable_list.begin(), selectable_list.end(), target);
		if(it != selectable_list.end()){
			for(int k=0; k<selected_list.size(); k++){
				selectable_list[selected_list[k]]->setMode(0);
			}
			selected_list.clear();
			selected_list.push_back((int)(it - selectable_list.begin()));
			model_list[no]->setMode(2);
			Label::setTarget(target);
		}
	}
}
