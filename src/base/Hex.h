
#ifndef HEX_H
#define HEX_H

#include <iostream>
#include <string>
#include "Road.h"
#include "Building.h"
#include "MyEnum.h"
#include "Player.h"

class Hex {

public:
	//enum Resource {water, wood, brick, wheat, wool, ore, desert};

	Hex();
	Hex(MyEnum::Resource resource);
	void setEdges(Road *edgeNW, Road *edgeNE, Road *edgeR, Road *edgeSE,
        Road *edgeSW, Road *edgeL);
    void setVertices(Building *vertexTL, Building *vertexT, Building *vertexTR,
        Building *vertexBR, Building *vertexB, Building *vertexBL);

    std::vector<Building *> getVertices();

    bool isCity(MyEnum::BuildingType type);
    void wasRolled(Player playerList[]);

    void setValue(int value);
    int getValue();

    void setThief(bool flag);
    
    std::string print();
    std::string testNull(Road *edge);
    std::string testNull(Building *vert);
    void initEdges();
    void initVertices();

    MyEnum::Resource getResource();

private:
	Road *edgeNW;
    Road *edgeNE;
	Road *edgeR;
	Road *edgeSE;
	Road *edgeSW;
	Road *edgeL;

	Building *vertexTL;
	Building *vertexT;
	Building *vertexTR;
	Building *vertexBR;
	Building *vertexB;
	Building *vertexBL;

	MyEnum::Resource resource;

	int value;

	bool thief;
};

#endif