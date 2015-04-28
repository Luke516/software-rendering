/*
 * CommandLine.h
 *
 *  Created on: 2015¦~4¤ë1¤é
 *      Author: LukeWu
 */

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include "Label.h"

class CommandLine :public Label {
public:
	CommandLine(float _x, float _y, std::string _text);
	virtual ~CommandLine();
	virtual void keyActive(int key, int action);
	virtual void setMode(int _mode){mode = 0;};
	virtual void render(Renderer *renderer, bool view_changed);
	bool getActive(){return active;}
	std::string getCommand(){return command;}
private:
	bool active;
	std::string command;
};

#endif /* COMMANDLINE_H_ */
