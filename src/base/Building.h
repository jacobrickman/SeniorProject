

#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <iostream>
#include "MyEnum.h"

class Building {
	
	MyEnum::Color color;
	MyEnum::BuildingType type;
	MyEnum::Port port;

public:
	Building();
	Building(MyEnum::Color color, MyEnum::BuildingType type, MyEnum::Port port);

	std::string print();
	//std::string typeToString(MyEnum::BuildingType type);
	void setPort(MyEnum::Port port);
	MyEnum::Port getPort();
	bool upgrade();
	bool freeSpace();
	MyEnum::Color getColor();
	MyEnum::BuildingType getType();
};

#endif