
#pragma once

#ifndef LAB471_SHAPE_H_INCLUDED
#define LAB471_SHAPE_H_INCLUDED

#include <string>
#include <vector>
#include <memory>

#include <glm/gtc/type_ptr.hpp>
#include "tiny_obj_loader.h"
#include "MatrixStack.h"

#define TINYOBJLOADER_IMPLEMENTATION

class Program;

class Shape
{

public:

	void loadMesh(const std::string &meshName);
	void init();
	void resize();
	glm::vec3 newFurthestPoint(glm::mat4 M);
	bool wasHit(glm::vec3 ray, glm::vec3 view, glm::mat4 M);
	void draw(const std::shared_ptr<Program> prog);
	void draw(const std::shared_ptr<Program> prog, int num);
	void draw(const std::shared_ptr<Program> prog, int shapeStart, int shapeEnd);

	std::vector<float> triangleNorm(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	void calculateNorms();
	float vectorLength(float x, float y, float z);

	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 center;
	glm::vec3 furthestPoint;
	float radius;

	std::vector<tinyobj::shape_t> shapes;

private:

	std::vector<unsigned int> eleBuf;
	std::vector<float> posBuf;
	std::vector<float> norBuf;
	std::vector<float> texBuf;
	std::vector<float> tmpBuf;

	//float *posBuf;

	std::vector<unsigned int> eleBufID = std::vector<unsigned int>(30);
	std::vector<unsigned int> posBufID = std::vector<unsigned int>(30);
	std::vector<unsigned int> norBufID = std::vector<unsigned int>(30);
	std::vector<unsigned int> texBufID = std::vector<unsigned int>(30);
	std::vector<unsigned int> vaoID = std::vector<unsigned int>(30);

	//unsigned int eleBufID = 0;
	//unsigned int posBufID = 0;
	//unsigned int norBufID = 0;
	//unsigned int texBufID = 0;
	//unsigned int vaoID = 0;

};

#endif // LAB471_SHAPE_H_INCLUDED
