/*
 * ObjLoader.cpp
 *
 *  Created on: 2015¦~3¤ë12¤é
 *      Author: LukeWu
 */

#include "ObjLoader.h"


#include "ObjLoader.h"
#include <glm/glm.hpp>
#include <cstdio>
#include <cstring>

ObjLoader::ObjLoader():loaded_model_count(0)
{
}

bool ObjLoader::LoadObjFromFileWithAABB(const char* path,std::vector<float>* vertices,
		std::vector<float>* uvs,std::vector<float>* normals,
		glm::vec3* aabb_bottom_left, glm::vec3* aabb_top_right) {

	char lineHeader[1000],line[1000];
	std::vector<float> temp_vertices;
	std::vector<float> temp_uvs;
	std::vector<float> temp_normals;
	std::vector<unsigned int> indices;
	float vertex[3], uv[2], normal[3];
	float xmin=10000,xmax=-10000,ymin=10000,ymax=-10000,zmin=10000,zmax=-10000;
	unsigned int index[3][3],f=0;

	FILE* objfile = fopen(path, "r");
	if (objfile == NULL) {
		printf("ObjLoader : Impossible to open the file !\n");
		return false;
	}
	while (fscanf(objfile, "%s", lineHeader) != EOF) {
		if (strcmp(lineHeader, "v") == 0) {

			fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);

			if (vertex[0] > xmax)xmax = vertex[0];
			if (vertex[0] < xmin)xmin = vertex[0];
			if (vertex[1] > ymax)ymax = vertex[1];
			if (vertex[1] < ymin)ymin = vertex[1];
			if (vertex[2] > zmax)zmax = vertex[2];
			if (vertex[2] < zmin)zmin = vertex[2];

			temp_vertices.push_back(vertex[0]);
			temp_vertices.push_back(vertex[1]);
			temp_vertices.push_back(vertex[2]);
		}
		else if (strcmp(lineHeader, "vt") == 0) {

			fscanf(objfile, "%f %f \n", &uv[0], &uv[1]);
			temp_uvs.push_back(uv[0]);
			temp_uvs.push_back(uv[1]);
		}
		else if (strcmp(lineHeader, "vn") == 0) {

			fscanf(objfile, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_normals.push_back(normal[0]);
			temp_normals.push_back(normal[1]);
			temp_normals.push_back(normal[2]);
		}
		else if (strcmp(lineHeader, "f") == 0) {

			int matches = fscanf(objfile, "%u/%u/%u %u/%u/%u %u/%u/%u",
					&index[0][0], &index[1][0], &index[2][0], &index[0][1],
					&index[1][1], &index[2][1], &index[0][2], &index[1][2],
					&index[2][2]);
			//printf("matches=%d,f=%u\n",matches,f++);
			//printf("%u/%u/%u %u/%u/%u %u/%u/%u\n",

			if (matches != 9) {
				printf("File can't be read by our simple parser ( Try exporting with other options\n");
				return false;
			}
			indices.push_back(index[0][0]);
			indices.push_back(index[1][0]);
			indices.push_back(index[2][0]);
			indices.push_back(index[0][1]);
			indices.push_back(index[1][1]);
			indices.push_back(index[2][1]);
			indices.push_back(index[0][2]);
			indices.push_back(index[1][2]);
			indices.push_back(index[2][2]);
		}
	}
	for (unsigned int i = 0; i < indices.size(); i += 3) {
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 1]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 2]);
		uvs->push_back(temp_uvs[(indices[i + 1] - 1) * 2]);
		uvs->push_back(temp_uvs[(indices[i + 1] - 1) * 2+1]);
		normals->push_back(temp_normals[(indices[i + 2] - 1) * 3]);
		normals->push_back(temp_normals[(indices[i + 2] - 1) * 3 + 1]);
		normals->push_back(temp_normals[(indices[i + 2] - 1) * 3 + 2]);

	}
	//printf("aabb_bottom_left: %f, %f, %f\n",xmin, ymin, zmin);
	//printf("aabb_top_right: %f, %f, %f\n",xmax, ymax, zmax);
	*aabb_bottom_left = glm::vec3(xmin, ymin, zmin);
	*aabb_top_right = glm::vec3(xmax, ymax, zmax);
	return true;
}

bool ObjLoader::LoadObjFromFile(const char* path,std::vector<float>* vertices,
		std::vector<float>* uvs,std::vector<float>* normals) {
	char lineHeader[1000];
	std::vector<float> temp_vertices;
	std::vector<float> temp_uvs;
	std::vector<float> temp_normals;
	std::vector<unsigned int> indices;
	float vertex[3], uv[2], normal[3];
	unsigned int index[3][3],f=0;

	FILE* objfile = fopen(path, "r");
	if (objfile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (fscanf(objfile, "%s", lineHeader) != EOF) {
		if (strcmp(lineHeader, "v") == 0) {

			fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);

			temp_vertices.push_back(vertex[0]);
			temp_vertices.push_back(vertex[1]);
			temp_vertices.push_back(vertex[2]);
		}
		else if (strcmp(lineHeader, "vt") == 0) {

			fscanf(objfile, "%f %f \n", &uv[0], &uv[1]);
			temp_uvs.push_back(uv[0]);
			temp_uvs.push_back(uv[1]);
		}
		else if (strcmp(lineHeader, "vn") == 0) {

			fscanf(objfile, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_normals.push_back(normal[0]);
			temp_normals.push_back(normal[1]);
			temp_normals.push_back(normal[2]);
		}
		else if (strcmp(lineHeader, "f") == 0) {

			int matches = fscanf(objfile, "%u/%u/%u %u/%u/%u %u/%u/%u",
					&index[0][0], &index[1][0], &index[2][0], &index[0][1],
					&index[1][1], &index[2][1], &index[0][2], &index[1][2],
					&index[2][2]);
			/*printf("matches=%d,f=%u\n",matches,f++);
			printf("%u/%u/%u %u/%u/%u %u/%u/%u\n",
					index[0][0], index[1][0], index[2][0], index[0][1],
					index[1][1], index[2][1], index[0][2], index[1][2],
					index[2][2]);*/
			if (matches != 9) {
				printf("File can't be read by our simple parser ( Try exporting with other options\n");
				return false;
			}
			indices.push_back(index[0][0]);
			indices.push_back(index[1][0]);
			indices.push_back(index[2][0]);
			indices.push_back(index[0][1]);
			indices.push_back(index[1][1]);
			indices.push_back(index[2][1]);
			indices.push_back(index[0][2]);
			indices.push_back(index[1][2]);
			indices.push_back(index[2][2]);
		}
	}
	for (unsigned int i = 0; i < indices.size(); i += 3) {
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 1]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 2]);
		uvs->push_back(temp_uvs[(indices[i + 1] - 1) * 2]);
		uvs->push_back(temp_uvs[(indices[i + 1] - 1) * 2+1]);
		normals->push_back(temp_normals[(indices[i + 2] - 1) * 3]);
		normals->push_back(temp_normals[(indices[i + 2] - 1) * 3 + 1]);
		normals->push_back(temp_normals[(indices[i + 2] - 1) * 3 + 2]);

	}
	return true;
}


bool ObjLoader::LoadObjFromFileVertexAndNormal(const char* path,std::vector<float>* vertices
		,std::vector<float>* normals, glm::vec3* aabb_bottom_left, glm::vec3* aabb_top_right) {

	char lineHeader[1000],line[1000];
	std::vector<float> temp_vertices;
	std::vector<float> temp_normals;
	std::vector<unsigned int> indices,index;
	float vertex[3], normal[3];
	float xmin=10000,xmax=-10000,ymin=10000,ymax=-10000,zmin=10000,zmax=-10000;
	unsigned int f=0,i,count=0;
	unsigned int temp_v, temp_n;

	FILE* objfile = fopen(path, "r");
	if (objfile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (fscanf(objfile, "%s", lineHeader) != EOF) {
		if (strcmp(lineHeader, "v") == 0) {

			fscanf(objfile, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			temp_vertices.push_back(vertex[0]);
			temp_vertices.push_back(vertex[1]);
			temp_vertices.push_back(vertex[2]);

			if (vertex[0] > xmax)xmax = vertex[0];
			if (vertex[0] < xmin)xmin = vertex[0];
			if (vertex[1] > ymax)ymax = vertex[1];
			if (vertex[1] < ymin)ymin = vertex[1];
			if (vertex[2] > zmax)zmax = vertex[2];
			if (vertex[2] < zmin)zmin = vertex[2];
		}
		else if (strcmp(lineHeader, "vn") == 0) {

			fscanf(objfile, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_normals.push_back(normal[0]);
			temp_normals.push_back(normal[1]);
			temp_normals.push_back(normal[2]);
		}
		/*else if (strcmp(lineHeader, "f") == 0) {

			int matches = fscanf(objfile, "%u//%u %u//%u %u//%u",
					&index[0][0], &index[2][0], &index[0][1],&index[2][1], &index[0][2], &index[2][2]);
			printf("matches=%d,f=%u\n",matches,f++);

			if (matches != 6) {
				printf("File can't be read by our simple parser ( Try exporting with other options\n");
				return false;
			}
			indices.push_back(index[0][0]);
			indices.push_back(index[2][0]);
			indices.push_back(index[0][1]);
			indices.push_back(index[2][1]);
			indices.push_back(index[0][2]);
			indices.push_back(index[2][2]);
		}*/
		else if (strcmp(lineHeader, "f") == 0) {
			fgets(line, 1000, objfile);
			printf("line : %s",line);
			count = 0;
			for(i=0; i<strlen(line); i++){
				if(i==0 || line[i]==' '){
					sscanf(line+i,"%u /%u",&temp_v, &temp_n);
					index.push_back(temp_v);
					index.push_back(temp_n);
					count++;
					if(count>3){
						index.push_back(index[0]);
						index.push_back(index[0 + 1]);
						index.push_back(index[(count-1-1)*2]);
						index.push_back(index[(count-1-1)*2 + 1]);
					}
				}
			}

			for(i=0; i<index.size(); i++){
				indices.push_back(index[i]);
			}
			index.clear();
			/*int matches = fscanf(objfile, "%u %u %u",&index[0][0], &index[0][1], &index[0][2]);
			if (matches != 3) {
				printf("File can't be read by our simple parser ( Try exporting with other options\n");
				return false;
			}*/
			//indices.push_back(index[0][0]);
			//indices.push_back(index[0][1]);
			//indices.push_back(index[0][2]);
		}
	}
	for (unsigned int i = 0; i < indices.size(); i+=2) {
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 1]);
		vertices->push_back(temp_vertices[(indices[i] - 1) * 3 + 2]);
		normals->push_back(temp_normals[(indices[i + 1] - 1) * 3]);
		normals->push_back(temp_normals[(indices[i + 1] - 1) * 3 + 1]);
		normals->push_back(temp_normals[(indices[i + 1] - 1) * 3 + 2]);
	}
	*aabb_bottom_left = glm::vec3(xmin, ymin, zmin);
	*aabb_top_right = glm::vec3(xmax, ymax, zmax);
	return true;
}


bool ObjLoader::LoadObjFromFileVertexOnly(const char* path,std::vector<float>* vertices, glm::vec3* aabb_bottom_left, glm::vec3* aabb_top_right)
{

	int i,count;
	unsigned int temp;
	char lineHeader[1000],line[1000];
	std::vector<float> temp_vertices;
	std::vector<float> temp_uvs;
	std::vector<float> temp_normals;
	std::vector<unsigned int> indices, index;
	float vertex[3], uv[2], normal[3];
	float xmin=10000,xmax=-10000,ymin=10000,ymax=-10000,zmin=10000,zmax=-10000;
	unsigned int f=0;

	FILE* objfile = fopen(path, "r");
	if (objfile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (fscanf(objfile, "%s", lineHeader) != EOF) {
		if (strcmp(lineHeader, "v") == 0) {

			fscanf(objfile, "%f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			temp_vertices.push_back(vertex[0]);
			temp_vertices.push_back(vertex[1]);
			temp_vertices.push_back(vertex[2]);

			if (vertex[0] > xmax)xmax = vertex[0];
			if (vertex[0] < xmin)xmin = vertex[0];
			if (vertex[1] > ymax)ymax = vertex[1];
			if (vertex[1] < ymin)ymin = vertex[1];
			if (vertex[2] > zmax)zmax = vertex[2];
			if (vertex[2] < zmin)zmin = vertex[2];
		}
		else if (strcmp(lineHeader, "vt") == 0) {

			fscanf(objfile, "%f %f ", &uv[0], &uv[1]);
			temp_uvs.push_back(uv[0]);
			temp_uvs.push_back(uv[1]);
		}
		else if (strcmp(lineHeader, "vn") == 0) {

			fscanf(objfile, "%f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			temp_normals.push_back(normal[0]);
			temp_normals.push_back(normal[1]);
			temp_normals.push_back(normal[2]);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			fgets(line, 1000, objfile);
			printf("line : %s",line);
			count = 0;
			for(i=0; i<strlen(line); i++){
				if(i==0 || line[i]==' '){
					sscanf(line+i,"%u",&temp);
					index.push_back(temp);
					count++;
					if(count>3){
						index.push_back(index[0]);
						index.push_back(index[count-2]);
					}
				}
			}

			for(i=0; i<index.size(); i++){
				indices.push_back(index[i]);
			}
			index.clear();
			/*int matches = fscanf(objfile, "%u %u %u",&index[0][0], &index[0][1], &index[0][2]);
			if (matches != 3) {
				printf("File can't be read by our simple parser ( Try exporting with other options\n");
				return false;
			}*/
			//indices.push_back(index[0][0]);
			//indices.push_back(index[0][1]);
			//indices.push_back(index[0][2]);
		}
	}
	for(unsigned int i=0;i<indices.size();i++){
		vertices->push_back(temp_vertices[(indices[i]-1)*3]);
		vertices->push_back(temp_vertices[(indices[i]-1)*3+1]);
		vertices->push_back(temp_vertices[(indices[i]-1)*3+2]);
	}
	*aabb_bottom_left = glm::vec3(xmin, ymin, zmin);
	*aabb_top_right = glm::vec3(xmax, ymax, zmax);
	return true;
}



