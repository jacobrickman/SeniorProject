#include "Shape.h"
#include <iostream>
#include <cassert>

#include "GLSL.h"
#include "Program.h"

using namespace std;

void Shape::loadMesh(const string &meshName)
{
	// Load geometry
	// Some obj files contain material information.
	// We'll ignore them for this assignment.
	vector<tinyobj::material_t> objMaterials;
	string errStr;
	bool rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str());

	if (! rc)
	{
		cerr << errStr << endl;
	}
}


void Shape::resize()
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	float scaleX, scaleY, scaleZ;
	float shiftX, shiftY, shiftZ;
	float epsilon = 0.001f;

	minX = minY = minZ = 1.1754E+38F;
	maxX = maxY = maxZ = -1.1754E+38F;

	for (int i = 0; i < shapes.size(); i++)
	{
		posBuf = shapes[i].mesh.positions;

		// Go through all vertices to determine min and max of each dimension
		for (size_t v = 0; v < posBuf.size() / 3; v++)
		{
			if (posBuf[3*v+0] < minX) minX = posBuf[3*v+0];
			if (posBuf[3*v+0] > maxX) maxX = posBuf[3*v+0];

			if (posBuf[3*v+1] < minY) minY = posBuf[3*v+1];
			if (posBuf[3*v+1] > maxY) maxY = posBuf[3*v+1];

			if (posBuf[3*v+2] < minZ) minZ = posBuf[3*v+2];
			if (posBuf[3*v+2] > maxZ) maxZ = posBuf[3*v+2];
		}
	}

	// From min and max compute necessary scale and shift for each dimension
	float maxExtent, xExtent, yExtent, zExtent;
	xExtent = maxX-minX;
	yExtent = maxY-minY;
	zExtent = maxZ-minZ;
	if (xExtent >= yExtent && xExtent >= zExtent)
	{
		maxExtent = xExtent;
	}
	if (yExtent >= xExtent && yExtent >= zExtent)
	{
		maxExtent = yExtent;
	}
	if (zExtent >= xExtent && zExtent >= yExtent)
	{
		maxExtent = zExtent;
	}
	scaleX = 2.0f / maxExtent;
	shiftX = minX + (xExtent / 2.0f);
	scaleY = 2.0f / maxExtent;
	shiftY = minY + (yExtent / 2.0f);
	scaleZ = 2.0f / maxExtent;
	shiftZ = minZ + (zExtent / 2.0f);

	for (int i = 0; i < shapes.size(); i++)
	{
		posBuf = shapes[i].mesh.positions;

		// Go through all verticies shift and scale them
		for (size_t v = 0; v < posBuf.size() / 3; v++)
		{
			posBuf[3*v+0] = (posBuf[3*v+0] - shiftX) * scaleX;
			assert(posBuf[3*v+0] >= -1.0f - epsilon);
			assert(posBuf[3*v+0] <= 1.0f + epsilon);
			posBuf[3*v+1] = (posBuf[3*v+1] - shiftY) * scaleY;
			assert(posBuf[3*v+1] >= -1.0f - epsilon);
			assert(posBuf[3*v+1] <= 1.0f + epsilon);
			posBuf[3*v+2] = (posBuf[3*v+2] - shiftZ) * scaleZ;
			assert(posBuf[3*v+2] >= -1.0f - epsilon);
			assert(posBuf[3*v+2] <= 1.0f + epsilon);
		}
	}
}

glm::vec3 Shape::newFurthestPoint(glm::mat4 M)
{
	glm::vec3 tmpCenter = glm::vec3(M * glm::vec4(center, 1.0));
	glm::vec3 tmpFur;
	float tmpRad = 0;

	for (int i = 0; i < shapes.size(); i++)
	{
		tmpBuf = shapes[i].mesh.positions;

		for (size_t v = 0; v < posBuf.size() / 3; v++)
		{
			glm::vec3 original = glm::vec3(posBuf[3*v+0], posBuf[3*v+1], posBuf[3*v+2]);
			glm::vec3 tmp = glm::vec3(M * glm::vec4(original, 1.0));

			if (glm::distance(tmpCenter, tmp) > tmpRad)
			{
				tmpRad = glm::distance(tmpCenter, tmp);
				tmpFur = original;
			}
		}
	}

	return tmpFur;
}

bool Shape::wasHit(glm::vec3 ray, glm::vec3 eye, glm::mat4 M)
{
	glm::vec3 tmpCenter;
	glm::vec3 tmpFur = newFurthestPoint(M);
	glm::vec3 view;
	float tmpRad;

	tmpCenter = glm::vec3(M * glm::vec4(center, 1.0));
	tmpFur = glm::vec3(M * glm::vec4(tmpFur, 1.0));
	tmpRad = glm::distance(tmpCenter, tmpFur);

	//std::cout << "CenterX: " + std::to_string(tmpCenter.x) << std::endl;
	//std::cout << "CenterY: " + std::to_string(tmpCenter.y) << std::endl;
	//std::cout << "CenterZ: " + std::to_string(tmpCenter.z) << std::endl;
	//std::cout << "Radius: " + std::to_string(tmpRad) << std::endl;

	view = eye - tmpCenter;
	float b = glm::dot(ray, view);
	float c = glm::dot(view, view) - (tmpRad * tmpRad);

	//std::cout << "b^2 - c: " + std::to_string(b * b - c) << std::endl;

	if (b * b - c >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<float> Shape::triangleNorm(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
	std::vector<float> ret(3);

	//P2 - P1
	float aX = x2 - x1;
	float aY = y2 - y1;
	float aZ = z2 - z1;

	//P3 - P1
	float bX = x3 - x1;
	float bY = y3 - y1;
	float bZ = z3 - z1;

	//Cross prduct
	ret[0] = aY * bZ - aZ * bY;
	ret[1] = aZ * bX - aX * bZ;
	ret[2] = aX * bY - aY * bX;

	return ret;
}

void Shape::calculateNorms() {
	std::vector<float> triNorm;

	//per triangle
	for (int i = 0; i < eleBuf.size(); i += 3) {
		triNorm = triangleNorm(posBuf[eleBuf[i + 0] * 3 + 0], posBuf[eleBuf[i + 0] * 3 + 1], posBuf[eleBuf[i + 0] * 3 + 2],
								posBuf[eleBuf[i + 1] * 3 + 0], posBuf[eleBuf[i + 1] * 3 + 1], posBuf[eleBuf[i + 1] * 3 + 2],
								posBuf[eleBuf[i + 2] * 3 + 0], posBuf[eleBuf[i + 2] * 3 + 1], posBuf[eleBuf[i + 2] * 3 + 2]);
		
		norBuf[eleBuf[i] * 3 + 0] += triNorm[0];
		norBuf[eleBuf[i] * 3 + 1] += triNorm[1];
		norBuf[eleBuf[i] * 3 + 2] += triNorm[2];

		norBuf[eleBuf[i + 1] * 3 + 0] += triNorm[0];
		norBuf[eleBuf[i + 1] * 3 + 1] += triNorm[1];
		norBuf[eleBuf[i + 1] * 3 + 2] += triNorm[2];

		norBuf[eleBuf[i + 2] * 3 + 0] += triNorm[0];
		norBuf[eleBuf[i + 2] * 3 + 1] += triNorm[1];
		norBuf[eleBuf[i + 2] * 3 + 2] += triNorm[2];
	}

	for (int i = 0; i < norBuf.size(); i += 3) {
		float length = vectorLength(norBuf[i], norBuf[i + 1], norBuf[i + 2]);
		norBuf[i] = norBuf[i] / length;
		norBuf[i + 1] = norBuf[i + 1] / length;
		norBuf[i + 2] = norBuf[i + 2] / length;

		//std::cout << norBuf[i] << ", " << norBuf[i + 1] << ", " << norBuf[i + 2] << std::endl;
	}
}

float Shape::vectorLength(float x, float y, float z) {
	return sqrt(x * x + y * y + z * z);
}

void Shape::init()
{
	for (int i = 0; i < shapes.size(); i++)
	{
		posBuf = shapes[i].mesh.positions;
		norBuf = shapes[i].mesh.normals;
		texBuf = shapes[i].mesh.texcoords;
		eleBuf = shapes[i].mesh.indices;

		// Initialize the vertex array object
		glGenVertexArrays(1, &vaoID[i]);
		glBindVertexArray(vaoID[i]);

		// Send the position array to the GPU
		glGenBuffers(1, &posBufID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, posBufID[i]);
		glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), posBuf.data(), GL_STATIC_DRAW);

		// Send the normal array to the GPU
		if (norBuf.empty())
		{	
			norBuf = std::vector<float> (posBuf.size());
			calculateNorms();
		}
		else
		{
			glGenBuffers(1, &norBufID[i]);
			glBindBuffer(GL_ARRAY_BUFFER, norBufID[i]);
			glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), norBuf.data(), GL_STATIC_DRAW);
		}

		// Send the texture array to the GPU
		if (texBuf.empty())
		{
			texBufID[i] = 0;
		}
		else
		{
			glGenBuffers(1, &texBufID[i]);
			glBindBuffer(GL_ARRAY_BUFFER, texBufID[i]);
			glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), texBuf.data(), GL_STATIC_DRAW);
		}

		// Send the element array to the GPU
		glGenBuffers(1, &eleBufID[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, eleBuf.size()*sizeof(unsigned int), eleBuf.data(), GL_STATIC_DRAW);

		// Unbind the arrays
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		assert(glGetError() == GL_NO_ERROR);
	}
}

void Shape::draw(const shared_ptr<Program> prog)
{
	draw(prog, 0, shapes.size());
}

void Shape::draw(const shared_ptr<Program> prog, int num)
{
	draw(prog, num, num + 1);
}

void Shape::draw(const shared_ptr<Program> prog, int shapeStart, int shapeEnd)
{
	int h_pos, h_nor, h_tex;
	h_pos = h_nor = h_tex = -1;

	for (int i = shapeStart; i < shapeEnd; i++)
	{
		posBuf = shapes[i].mesh.positions;
		eleBuf = shapes[i].mesh.indices;

		glBindVertexArray(vaoID[i]);
		// Bind position buffer
		h_pos = prog->getAttribute("vertPos");
		GLSL::enableVertexAttribArray(h_pos);
		glBindBuffer(GL_ARRAY_BUFFER, posBufID[i]);
		glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);

		// Bind normal buffer
		h_nor = prog->getAttribute("vertNor");
		if (h_nor != -1 && norBufID[i] != 0)
		{
			GLSL::enableVertexAttribArray(h_nor);
			glBindBuffer(GL_ARRAY_BUFFER, norBufID[i]);
			glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
		}

		if (texBufID[i] != 0)
		{
			// Bind texcoords buffer
			h_tex = prog->getAttribute("vertTex");
			if (h_tex != -1 && texBufID[i] != 0)
			{
				GLSL::enableVertexAttribArray(h_tex);
				glBindBuffer(GL_ARRAY_BUFFER, texBufID[i]);
				glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);
			}
		}

		// Bind element buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID[i]);

		// Draw
		glDrawElements(GL_TRIANGLES, (int)eleBuf.size(), GL_UNSIGNED_INT, (const void *)0);

		// Disable and unbind
		if (h_tex != -1)
		{
			GLSL::disableVertexAttribArray(h_tex);
		}
		if (h_nor != -1)
		{
			GLSL::disableVertexAttribArray(h_nor);
		}
		GLSL::disableVertexAttribArray(h_pos);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
