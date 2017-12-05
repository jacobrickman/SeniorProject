

#ifndef MYENUM_H
#define MYENUM_H

#include <string>
#include <vector>
#include <iostream>

class MyEnum {

public:
	enum Resource {water = -1, wood = 0, brick = 1, wheat = 2, wool = 3, ore = 4, desert = 5};
	enum Color {red = 0, blue = 1, green = 2, white = 3, empty};
	enum BuildingType {settlement, city, none};
	enum Port {woodPort, brickPort, wheatPort, woolPort, orePort, genPort, noPort};
	enum DevCard {knight = 0, roadBuilding = 1, yearPlenty = 2, monopoly = 3, victoryPoint = 4};
	enum STATE {GAME, BUILDING, BUILD_SETTLEMENT, BUILD_ROAD, BUILD_CITY, PLACE_THIEF,
				 STEAL_FROM_PLAYERS, PICK_NUM_PLAYERS, SETUP, DEV_CARD, ROAD_BUILDING_1, 
				 ROAD_BUILDING_2, YEAR_PLENTY, YEAR_PLENTY_2, MONOPOLY};

	static std::string colorToString(MyEnum::Color c);
	static std::string resourceToString(MyEnum::Resource r);
	static std::string typeToString(MyEnum::BuildingType t);
	static std::string portToString(MyEnum::Port p);
	static std::string devcardToString(MyEnum::DevCard card);
	static std::string portToResourceString(MyEnum::Port p);
	static MyEnum::Resource portToResource(MyEnum::Port p);
	static std::string vectorToString(std::vector<int> v);
	static std::string vectorToString(std::vector<MyEnum::Resource> v);
	static std::string vectorToString(std::vector<std::string> v);
	static std::string vectorToString(std::vector<MyEnum::Port> v);
	static std::string vectorToString(std::vector<MyEnum::Color> color);
	static void print(std::string s);
	static void print(int i);
	static void print(std::vector<int> v);
	static bool roadVerts(int v1, int v2, int x, int y);
	static void zero(int arr[], int size);
};

#endif