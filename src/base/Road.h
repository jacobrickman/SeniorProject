

#ifndef ROAD_H
#define ROAD_H

#include <string>
#include "MyEnum.h"

class Road {


public:
    //enum Color {red, blue, green, white, empty};

	Road();
	Road(MyEnum::Color color);
	Road(MyEnum::Color color, Road *prev);
	std::string print();
	MyEnum::Color getColor();
	

    MyEnum::Color color;
	Road *prev;
	Road *next;
};

#endif