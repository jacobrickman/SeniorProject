

#include "Hex.h"

Hex::Hex()
{
	this->resource = MyEnum::water;
	value = -1;
	thief = false;
	initEdges();
	initVertices();
}

Hex::Hex(MyEnum::Resource resource)
{
	value = -1;
	this->resource = resource;
	thief = false;
	initEdges();
	initVertices();
}

void Hex::initEdges()
{
	this->edgeNW = NULL;
    this->edgeNE = NULL;
    this->edgeR = NULL;
    this->edgeSE = NULL;
    this->edgeSW = NULL;
    this->edgeL = NULL;
}

void Hex::initVertices()
{
	this->vertexTL = NULL;
    this->vertexT = NULL;
    this->vertexTR = NULL;
    this->vertexBR = NULL;
    this->vertexB = NULL;
    this->vertexBL = NULL;
}

void Hex::setEdges(Road *edgeNW, Road *edgeNE, Road *edgeR, Road *edgeSE,
        Road *edgeSW, Road *edgeL)
{
    this->edgeNW = edgeNW;
    this->edgeNE = edgeNE;
    this->edgeR = edgeR;
    this->edgeSE = edgeSE;
    this->edgeSW = edgeSW;
    this->edgeL = edgeL;
}

void Hex::setVertices(Building *vertexTL, Building *vertexT, Building *vertexTR,
        Building *vertexBR, Building *vertexB, Building *vertexBL)
{
    this->vertexTL = vertexTL;
    this->vertexT = vertexT;
    this->vertexTR = vertexTR;
    this->vertexBR = vertexBR;
    this->vertexB = vertexB;
    this->vertexBL = vertexBL;
}

void Hex::setThief(bool flag)
{
	thief = flag;
}

std::vector<Building *> Hex::getVertices()
{
	std::vector<Building *> ret = std::vector<Building *>();
	ret.push_back(vertexTL);
	ret.push_back(vertexT);
	ret.push_back(vertexTR);
	ret.push_back(vertexBR);
	ret.push_back(vertexB);
	ret.push_back(vertexBL);

	return ret;
}

bool Hex::isCity(MyEnum::BuildingType type)
{
	if (type == MyEnum::city)
		return true;

	return false;
}

void Hex::wasRolled(Player playerList[])
{
	if (!thief)
	{
		if (vertexTL->getColor() != MyEnum::empty)
		{
			std::vector<MyEnum::Resource> tl = std::vector<MyEnum::Resource>();
			tl.push_back(resource);
			if (isCity(vertexTL->getType()))
			{
				tl.push_back(resource);
			}

			MyEnum::print(MyEnum::colorToString(vertexTL->getColor()) + " gained " + MyEnum::vectorToString(tl));
			playerList[vertexTL->getColor()].gainResourceCards(tl);

		}

		if (vertexT->getColor() != MyEnum::empty)
		{
			std::vector<MyEnum::Resource> t = std::vector<MyEnum::Resource>();
			t.push_back(resource);
			if (isCity(vertexT->getType()))
			{
				t.push_back(resource);
			}

			MyEnum::print(MyEnum::colorToString(vertexT->getColor()) + " gained " + MyEnum::vectorToString(t));
			playerList[vertexT->getColor()].gainResourceCards(t);
		}

		if (vertexTR->getColor() != MyEnum::empty)
		{
			std::vector<MyEnum::Resource> tr = std::vector<MyEnum::Resource>();
			tr.push_back(resource);
			if (isCity(vertexTR->getType()))
			{
				tr.push_back(resource);
			}

			MyEnum::print(MyEnum::colorToString(vertexTR->getColor()) + " gained " + MyEnum::vectorToString(tr));
			playerList[vertexTR->getColor()].gainResourceCards(tr);
		}

		if (vertexBR->getColor() != MyEnum::empty)
		{
			std::vector<MyEnum::Resource> br = std::vector<MyEnum::Resource>();
			br.push_back(resource);
			if (isCity(vertexBR->getType()))
			{
				br.push_back(resource);
			}

			MyEnum::print(MyEnum::colorToString(vertexBR->getColor()) + " gained " + MyEnum::vectorToString(br));
			playerList[vertexBR->getColor()].gainResourceCards(br);
		}

		if (vertexB->getColor() != MyEnum::empty)
		{
			std::vector<MyEnum::Resource> b = std::vector<MyEnum::Resource>();
			b.push_back(resource);
			if (isCity(vertexB->getType()))
			{
				b.push_back(resource);
			}

			MyEnum::print(MyEnum::colorToString(vertexB->getColor()) + " gained " + MyEnum::vectorToString(b));
			playerList[vertexB->getColor()].gainResourceCards(b);
		}

		if (vertexBL->getColor() != MyEnum::empty)
		{
			std::vector<MyEnum::Resource> bl = std::vector<MyEnum::Resource>();
			bl.push_back(resource);
			if (isCity(vertexBL->getType()))
			{
				bl.push_back(resource);
			}

			MyEnum::print(MyEnum::colorToString(vertexBL->getColor()) + " gained " + MyEnum::vectorToString(bl));
			playerList[vertexBL->getColor()].gainResourceCards(bl);
		}
	}
	else
	{
		MyEnum::print("The thief covers the " + MyEnum::resourceToString(resource));
	}
}

std::string Hex::testNull(Road *edge)
{
	if (edge != NULL) {
		return edge->print();
	}
	else {
		return "IV";
	}
}

std::string Hex::testNull(Building *vert)
{
	if (vert != NULL) {
		return vert->print();
	}
	else {
		return "IV";
	}
}

std::string Hex::print()
{	
	return "    |" + testNull(edgeNW) + "| |" + testNull(vertexT) + "| |" + testNull(edgeNE) + "|\n"
			+ "  |" + testNull(vertexTL) + "|          |" + testNull(vertexTR) + "|\n"
			+ "|" + testNull(edgeL) + "|      " + MyEnum::resourceToString(resource) + "       |" + testNull(edgeR) + "|\n" 
			+ "  |" + testNull(vertexBL) + "|          |" + testNull(vertexBR) + "|\n"
			+ "    |" + testNull(edgeSW) + "| |" + testNull(vertexB) + "| |" + testNull(edgeSE) + "|\n";
	
}

MyEnum::Resource Hex::getResource()
{
	return resource;
}

int Hex::getValue()
{
	return value;
}

void Hex::setValue(int value)
{
	this->value = value;
}










