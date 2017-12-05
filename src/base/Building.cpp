

#include "Building.h"

Building::Building()
{
	this->color = MyEnum::empty;
	this->type = MyEnum::none;	
	this->port = MyEnum::noPort;
}

Building::Building(MyEnum::Color color, MyEnum::BuildingType type, MyEnum::Port port)
{
	this->color = color;
	this->type = type;
	this->port = port;
}

std::string Building::print()
{
	return MyEnum::typeToString(type);
}

/*
std::string Building::typeToString(MyEnum::BuildingType type)
{
	switch(type)
	{
		case MyEnum::settlement: return "Settlement";
		case MyEnum::city: return "City";
		case MyEnum::none: return "";
	}
}
*/

void Building::setPort(MyEnum::Port port)
{
	this->port = port;
}

MyEnum::Port Building::getPort()
{
	return port;
}

bool Building::upgrade()
{
	if (type == MyEnum::settlement)
	{
		type = MyEnum::city;
		return true;

	} else if (type == MyEnum::city)
		std::cout << "Can't upgrade a City" << std::endl;
	else
		std::cout << "No building to upgrade" << std::endl;

	return false;
}

bool Building::freeSpace()
{
	if (type == MyEnum::none)
		return true;
	
	return false;
}

MyEnum::Color Building::getColor()
{
	return color;
}

MyEnum::BuildingType Building::getType()
{
	return type;
}


