
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <stdio.h>
#include <iostream>
#include <vector>

#include "Hex.h"
#include "Road.h"
#include "Building.h"
#include "Player.h"
#include "MyEnum.h"
#include <glm/gtc/type_ptr.hpp>

class GameBoard {

	public:

	    GameBoard();
	    void initializeHexes();
	    void print();
	    void setHexEdges();
	    void setHexVertices();
	    Hex *getHex_normal(int x, int y);
	    Hex *getHex(int x, int y);
	    bool validHex(int i, int j);
	    bool notWater(int i, int j);

	    void diceRoll(Player playerList[], int roll);
	    std::vector<MyEnum::Color> placeThief(int x, int y);

	    //Checks if you can place your first or second settlement at the vert
	    bool validFirstSettlement(int v);

	    std::vector<int> possibleSettlementPositions(MyEnum::Color color);
	    std::vector<int> possibleRoadPositions(MyEnum::Color color);
	    std::vector<int> possibleCityPositions(MyEnum::Color color);

	    //Checks if you can place a building at the vert
	    bool validBuildingVertex(int v, MyEnum::Color color);
	    bool validCityVertex(int v, MyEnum::Color color);

	    std::vector<int> adjacentVerts(bool top, int i, int min, int max, bool doOffset, int offset);
	    std::vector<int> getAdjacentVertices(int vertIndex);
	    void defaultResources();
	    void defaultValues();
	    void defaultPorts();

	    std::vector<MyEnum::Port> getPlayerPorts(MyEnum::Color color);

	    void buildFirstSettlement(int vertIndex, MyEnum::Color color);
	    bool buildASettlement(int vertIndex, MyEnum::Color color);
	    bool buildACity(int vertIndex, MyEnum::Color color);

	    bool validBuildingEdge(int edgeIndex, MyEnum::Color color);
	    bool buildARoad(int edgeIndex, MyEnum::Color color);
	    std::vector<int> roadsOffABuilding(int buildingIndex, MyEnum::Color playerColor);

	    std::vector<int> roadToVerts(int roadIndex);
	    int vertsToRoad(int vertIndex1, int vertIndex2);

	    std::vector<int> makeVector_2(int x, int y);
	    std::vector<int> makeVector_3(int x, int y, int z);

	    std::vector<Road> getEdges();
	    std::vector<Building> getVertices();
	    Hex *getThiefPos();

	    int vertToHex(int i);
	    int edgeToHex(int index);

	    int findLongestRoad(Player playerList[], int numPlayers, Player **longestRoad, int longestRoadCount);
	    int longestStart(int index, int counted[], MyEnum::Color color);
	    int longestEdgeDirected(int index, int counted[], MyEnum::Color color, int previousVert);
	    int longestVert(int vert, int counted[], MyEnum::Color color);

	    int getBuildingPoints(MyEnum::Color color);

	    std::vector<MyEnum::Resource> distributeFirstRoundResources(MyEnum::Color color);

	    std::vector<MyEnum::Resource> adjacentResource(int vertIndex);

	    glm::vec2 intToHex(int i);


	private:
	    Hex *board[7][7];
	    std::vector<Road> edges;
	    std::vector<Building> vertices;
	    std::vector<MyEnum::Resource> resourceList;
	    std::vector<int> valueList;
	    Hex *thief;
};

#endif