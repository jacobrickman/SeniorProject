

#include "Road.h"

Road::Road()
{
	this->color = MyEnum::empty;
	this->prev = NULL;
	this->next = NULL;
}

Road::Road(MyEnum::Color color)
{
	this->color = color;
	this->prev = NULL;
	this->next = NULL;
}

Road::Road(MyEnum::Color color, Road *prev)
{
	this->color = color;
	this->prev = prev;
	this->next = NULL;
}

std::string Road::print()
{
	return MyEnum::colorToString(color);
}

MyEnum::Color Road::getColor()
{
	return color;
}
