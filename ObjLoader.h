/*
 * ObjLoader.h
 *
 *  Created on: 2015¦~3¤ë12¤é
 *      Author: LukeWu
 */

#ifndef OBJLOADER_H_
#define OBJLOADER_H_
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ObjLoader {
public:
	ObjLoader();
	static bool LoadObjFromFileWithAABB(const char*,std::vector<float>*,std::vector<float>*,std::vector<float>*,
			glm::vec3*, glm::vec3*);
	static bool LoadObjFromFile(const char*,std::vector<float>*,std::vector<float>*,std::vector<float>*);
	static bool LoadObjFromFileVertexOnly(const char*,std::vector<float>*, glm::vec3* , glm::vec3* );
	static bool LoadObjFromFileVertexAndNormal(const char*,std::vector<float>*,std::vector<float>*,glm::vec3*, glm::vec3*);
private:
	unsigned int loaded_model_count;
	std::map<std::string,unsigned int> path_map;

};

#endif /* OBJLOADER_H_ */
