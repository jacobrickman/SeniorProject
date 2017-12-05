

#include "MyEnum.h"

std::string MyEnum::colorToString(Color c)
{
	switch(c)
	{
		case red: return "Red";
		case blue: return "Blue";
		case green: return "Green";
		case white: return "White";
		case empty: return "";
	}
}

std::string MyEnum::resourceToString(MyEnum::Resource r)
{
	switch(r)
	{
		case water: return "Water";
		case wood: return "Wood";
		case brick: return "Brick";
		case wheat: return "Wheat";
		case wool: return "Wool";
		case ore: return "Ore";
		case desert: return "Desert";
	}
}

std::string MyEnum::vectorToString(std::vector<int> v)
{
	std::string s = "";

	for (int i = 0; i < v.size(); i++)
	{
		s += std::to_string(v[i]) + ", ";
	}

	return s;
}

std::string MyEnum::vectorToString(std::vector<MyEnum::Resource> v)
{
	std::string s = "";

	for (int i = 0; i < v.size(); i++)
	{
		s += resourceToString(v[i]) + ", ";
	}

	return s;
}

std::string MyEnum::vectorToString(std::vector<std::string> v)
{
	std::string s = "";

	for (int i = 0; i < v.size(); i++)
	{
		s += v[i] + ", ";
	}

	return s;
}

std::string MyEnum::vectorToString(std::vector<MyEnum::Port> p)
{
	std::string s = "";

	for (int i = 0; i < p.size(); i++)
	{
		s += portToString(p[i]) + ", ";
	}

	return s;
}

std::string MyEnum::vectorToString(std::vector<MyEnum::Color> color)
{
	std::string s = "";

	for (int i = 0; i < color.size(); i++)
	{
		s += colorToString(color[i]) + ", ";
	}

	return s;
}

std::string MyEnum::typeToString(MyEnum::BuildingType t)
{
	switch(t)
	{
		case settlement: return "Settlement";
		case city: return "City";
		case none: return "None";
	}
}

std::string MyEnum::portToString(MyEnum::Port p)
{
	switch(p)
	{
		case woodPort: return "Wood Port";
		case brickPort: return "Brick Port";
		case wheatPort: return "Wheat Port";
		case woolPort: return "Wool Port";
		case orePort: return "Ore Port";
		//case genPort: return "General Port";
		case genPort: return "3 : 1 Port";
		case noPort: return "4: 1 Port";
	}
}

std::string MyEnum::portToResourceString(MyEnum::Port p)
{
	switch(p)
	{
		case woodPort: return "Wood";
		case brickPort: return "Brick";
		case wheatPort: return "Wheat";
		case woolPort: return "Wool";
		case orePort: return "Ore";
		//case genPort: return "General";
		case genPort: return "3:1 Port";
		case noPort: return "4:1 Port";
	}	
}

std::string MyEnum::devcardToString(MyEnum::DevCard card)
{
	switch(card)
	{
		case knight: return "Knight";
		case roadBuilding: return "Road Building";
		case yearPlenty: return "Year of Plenty";
		case monopoly: return "Monopoly";
		case victoryPoint: return "Victory Point";
	}
}

MyEnum::Resource MyEnum::portToResource(MyEnum::Port p)
{
	switch(p)
	{
		case woodPort: return wood;
		case brickPort: return brick;
		case wheatPort: return wheat;
		case woolPort: return wool;
		case orePort: return ore;
		//case genPort: return "General";
		case genPort: return water;
		case noPort: return water;
	}
}

void MyEnum::print(std::string s)
{
	std::cout << s << std::endl;
}

void MyEnum::print(int i)
{
	print(std::to_string(i));
}

void MyEnum::print(std::vector<int> v)
{
	print(vectorToString(v));
}

bool MyEnum::roadVerts(int v1, int v2, int x, int y)
{
	if (v1 == x && v2 == y)
	{
		return true;
	}
	else if (v1 == y && v2 == x)
	{
		return true;
	}

	return false;
}

void MyEnum::zero(int arr[], int size)
{
	for (int i = 0; i < size; i++)
	{
		arr[i] = 0;
	}
}




