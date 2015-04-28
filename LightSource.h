/*
 * LightSource.h
 *
 *  Created on: 2015¦~4¤ë27¤é
 *      Author: LukeWu
 */

#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "SelectableObject.h"
#include "Light.h"

class LightSource : SelectableObject{
public:
	LightSource(float x_=0.0, float y_=0.0, float z_=0.0, float r_=0.0, float g_=0.0, float b_=0.0);
	virtual ~LightSource();
	virtual void render(Renderer *renderer, bool view_changed);
	virtual void keyActive(int key, int action);
private:
	Light light;
};

#endif /* LIGHTSOURCE_H_ */
