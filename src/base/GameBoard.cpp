
#include "GameBoard.h"

GameBoard::GameBoard() {
	
    edges = std::vector<Road>(72);
    vertices = std::vector<Building>(54);
    resourceList = std::vector<MyEnum::Resource> (37);
    valueList = std::vector<int>(37);

    for (int i = 0; i < 72; i++)
    {
        if (i < 37) {
            edges[i] = Road(MyEnum::empty);
            vertices[i] = Building();
        }
        else if (i < 54) {
            edges[i] = Road(MyEnum::empty);
            vertices[i] = Building();
        }
        else {
            edges[i] = Road(MyEnum::empty);
        }
    }

    defaultResources();
    defaultValues();
    initializeHexes();
    defaultPorts();
}

std::vector<Road> GameBoard::getEdges()
{
    return edges;
}

std::vector<Building> GameBoard::getVertices()
{
    return vertices;
}

int GameBoard::vertToHex(int index)
{
    if (index < 0 || index > 53)
    {
        MyEnum::print("Incorrect vertex passed to vertToHex: " + std::to_string(index));
        return -1;
    }

    if (index < 7)
    {
        if (index < 3)
        {
            return 5;
        }
        else if (index < 5)
        {
            return 6;
        }
        else
        {
            return 7;
        }
    }
    else if (index < 16)
    {
        if (index < 10)
        {
            return 10;
        }
        else if (index < 12)
        {
            return 11;
        }
        else if (index < 14)
        {
            return 12;
        }
        else 
        {
            return 13;
        }

    }
    else if (index < 28 || index == 37)
    {
        if (index < 19 || index == 27)
        {
            return 16;
        }
        else if (index < 21)
        {
            return 17;
        }
        else if (index < 23)
        {
            return 18;
        }
        else if (index < 25)
        {
            return 19;
        }
        else if (index < 27 || index == 37)
        {
            return 20;
        }

    }
    else if (index < 39 || index == 46)
    {
        if (index < 31 || index == 38)
        {
            return 23;
        }
        else if (index < 33)
        {
            return 24;
        }
        else if (index < 35)
        {
            return 25;
        }
        else if (index < 37 || index == 46)
        {
            return 26;
        }
    }
    else
    {
        if (index < 42 || index == 47 || index == 48 || index == 49)
        {
            return 29;
        }
        else if (index < 44 || index == 50 || index == 51)
        {
            return 30;
        }
        else if (index < 46 || index == 52 || index == 53)
        {
            return 31;
        }
    }

    return -1;
}

int GameBoard::edgeToHex(int index)
{
    if (index < 0 || index > 71)
    {
        MyEnum::print("Incorrect edge passed to edgeToHex: " + std::to_string(index));
        return -1;
    }

    if (index < 10)
    {
        if (index < 2 || index == 6 || index == 7)
        {
            return 5;
        }
        else if (index < 4 || index == 8)
        {
            return 6;
        }
        else
        {
            return 7;
        }
    }
    else if (index < 23)
    {
        if (index < 12 || index == 18 || index == 19)
        {
            return 10;
        }
        else if (index < 14 || index == 20)
        {
            return 11;
        }
        else if (index < 16 || index == 21)
        {
            return 12;
        }
        else 
        {
            return 13;
        }
    }
    else if (index < 40 || index == 48)
    {
        if (index < 25 || index == 33 || index == 34 || index == 39)
        {
            return 16;
        }
        else if (index < 27 || index == 35)
        {
            return 17;
        }
        else if (index < 29 || index == 36)
        {
            return 18;
        }
        else if (index < 31 || index == 37)
        {
            return 19;
        }
        else
        {
            return 20;
        }
    }
    else if (index < 55 || index == 61)
    {
        if (index < 42 || index == 49 || index == 50 || index == 54)
        {
            return 23;
        }
        else if (index < 44 || index == 51)
        {
            return 24;
        }
        else if (index < 46 || index == 52)
        {
            return 25;
        }
        else
        {
            return 26;
        }
    }
    else 
    {
        if (index < 57 || index == 62 || index == 63 || index == 66 || index == 67)
        {
            return 29;
        }
        else if (index < 59 || index == 64 || index == 68 || index == 69)
        {
            return 30;
        }
        else
        {
            return 31;
        }
    }
}

void GameBoard::initializeHexes()
{
	int index = 0;
    
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (!((i == 0 && (j == 0 || j == 1 || j == 2)) ||
                    (i == 1 && (j == 0 || j == 1)) ||
                    (i == 2 && j == 0) ||
                    (i == 4 && j == 6) ||
                    (i == 5 && (j == 5 || j == 6)) ||
                    (i == 6 && (j == 4 || j == 5 || j == 6)) ))
            {
                board[i][j] = new Hex(resourceList[index]);
                board[i][j]->setValue(valueList[index]);
                if (index == 18)
                {
                    board[i][j]->setThief(true);
                    thief = board[i][j];
                }

                index++;
            }
            else
            	board[i][j] = NULL;
        }
    }

    setHexEdges();
    setHexVertices();
}

void GameBoard::print()
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (board[i][j])
				std::cout << getHex_normal(i, j)->print();
			else
				std::cout << "|E|";
		}
		std::cout << std::endl;
	}
}


void GameBoard::setHexEdges()
{
    //setEdges(NW,     NE,      R,     SE,    SW,     L)
    board[0][3]->setEdges(0, 0, 0, &edges[0], 0, 0);
    board[0][4]->setEdges(0, 0, 0, &edges[2], &edges[1], 0);
    board[0][5]->setEdges(0, 0, 0, &edges[4], &edges[3], 0);
    board[0][6]->setEdges(0, 0, 0, 0, &edges[5], 0);
    board[1][2]->setEdges(0, 0, &edges[6], &edges[10], 0, 0);
    board[1][3]->setEdges(&edges[0], &edges[1], &edges[7], &edges[12], &edges[11], &edges[6]);
    board[1][4]->setEdges(&edges[2], &edges[3], &edges[8], &edges[14], &edges[13], &edges[7]);
    board[1][5]->setEdges(&edges[4], &edges[5], &edges[9], &edges[16], &edges[15], &edges[8]);
    board[1][6]->setEdges(0, 0, 0, 0, &edges[17], &edges[9]);
    board[2][1]->setEdges(0, 0, &edges[18], &edges[23], 0, 0);
    board[2][2]->setEdges(&edges[10], &edges[11], &edges[19], &edges[25], &edges[24], &edges[18]);
    board[2][3]->setEdges(&edges[12], &edges[13], &edges[20], &edges[27], &edges[26], &edges[19]);
    board[2][4]->setEdges(&edges[14], &edges[15], &edges[21], &edges[29], &edges[28], &edges[20]);
    board[2][5]->setEdges(&edges[16], &edges[17], &edges[22], &edges[31], &edges[30], &edges[21]);
    board[2][6]->setEdges(0, 0, 0, 0, &edges[32], &edges[22]);
    board[3][0]->setEdges(0, 0, &edges[33], 0, 0, 0);
    board[3][1]->setEdges(&edges[23], &edges[24], &edges[34], &edges[40], &edges[39], &edges[33]);
    board[3][2]->setEdges(&edges[25], &edges[26], &edges[35], &edges[42], &edges[41], &edges[34]);
    board[3][3]->setEdges(&edges[27], &edges[28], &edges[36], &edges[44], &edges[43], &edges[35]);
    board[3][4]->setEdges(&edges[29], &edges[30], &edges[37], &edges[46], &edges[45], &edges[36]);
    board[3][5]->setEdges(&edges[31], &edges[32], &edges[38], &edges[48], &edges[47], &edges[37]);
    board[3][6]->setEdges(0, 0, 0, 0, 0, &edges[38]);
    board[4][0]->setEdges(0, &edges[39], &edges[49], 0, 0, 0);
    board[4][1]->setEdges(&edges[40], &edges[41], &edges[50], &edges[55], &edges[54], &edges[49]);
    board[4][2]->setEdges(&edges[42], &edges[43], &edges[51], &edges[57], &edges[56], &edges[50]);
    board[4][3]->setEdges(&edges[44], &edges[45], &edges[52], &edges[59], &edges[58], &edges[51]);
    board[4][4]->setEdges(&edges[46], &edges[47], &edges[53], &edges[61], &edges[60], &edges[52]);
    board[4][5]->setEdges(&edges[48], 0, 0, 0, 0, &edges[53]);
    board[5][0]->setEdges(0, &edges[54], &edges[62], 0, 0, 0);
    board[5][1]->setEdges(&edges[55], &edges[56], &edges[63], &edges[67], &edges[66], &edges[62]);
    board[5][2]->setEdges(&edges[57], &edges[58], &edges[64], &edges[69], &edges[68], &edges[63]);
    board[5][3]->setEdges(&edges[59], &edges[60], &edges[65], &edges[71], &edges[70], &edges[64]);
    board[5][4]->setEdges(&edges[61], 0, 0, 0, 0, &edges[65]);
    board[6][0]->setEdges(0, &edges[66], 0, 0, 0, 0);
    board[6][1]->setEdges(&edges[67], &edges[68], 0, 0, 0, 0);
    board[6][2]->setEdges(&edges[69], &edges[70], 0, 0, 0, 0);
    board[6][3]->setEdges(&edges[71], 0, 0, 0, 0, 0);
    
}

void GameBoard::setHexVertices()
{
    //setVertices( TL, T, TR, BR, B, BL
    board[0][3]->setVertices(0, 0, 0, &vertices[1], &vertices[0], 0);
    board[0][4]->setVertices(0, 0, 0, &vertices[3], &vertices[2], &vertices[1]);
    board[0][5]->setVertices(0, 0, 0, &vertices[5], &vertices[4], &vertices[3]);
    board[0][6]->setVertices(0, 0, 0, 0, &vertices[6], &vertices[5]);
    board[1][2]->setVertices(0, 0, &vertices[0], &vertices[8], &vertices[7], 0);
    board[1][3]->setVertices(&vertices[0], &vertices[1], &vertices[2], &vertices[10], &vertices[9], &vertices[8]);
    board[1][4]->setVertices(&vertices[2], &vertices[3], &vertices[4], &vertices[12], &vertices[11], &vertices[10]);
    board[1][5]->setVertices(&vertices[4], &vertices[5], &vertices[6], &vertices[14], &vertices[13], &vertices[12]);
    board[1][6]->setVertices(&vertices[6], 0, 0, 0, &vertices[15], &vertices[14]);
    board[2][1]->setVertices(0, 0, &vertices[7], &vertices[17], &vertices[16], 0);
    board[2][2]->setVertices(&vertices[7], &vertices[8], &vertices[9], &vertices[19], &vertices[18], &vertices[17]);
    board[2][3]->setVertices(&vertices[9], &vertices[10], &vertices[11], &vertices[21], &vertices[20], &vertices[19]);
    board[2][4]->setVertices(&vertices[11], &vertices[12], &vertices[13], &vertices[23], &vertices[22], &vertices[21]);
    board[2][5]->setVertices(&vertices[13], &vertices[14], &vertices[15], &vertices[25], &vertices[24], &vertices[23]);
    board[2][6]->setVertices(&vertices[15], 0, 0, 0, &vertices[26], &vertices[25]);
    board[3][0]->setVertices(0, 0, &vertices[16], &vertices[27], 0, 0);
    board[3][1]->setVertices(&vertices[16], &vertices[17], &vertices[18], &vertices[29], &vertices[28], &vertices[27]);
    board[3][2]->setVertices(&vertices[18], &vertices[19], &vertices[20], &vertices[31], &vertices[30], &vertices[29]);
    board[3][3]->setVertices(&vertices[20], &vertices[21], &vertices[22], &vertices[33], &vertices[32], &vertices[31]);
    board[3][4]->setVertices(&vertices[22], &vertices[23], &vertices[24], &vertices[35], &vertices[34], &vertices[33]);
    board[3][5]->setVertices(&vertices[24], &vertices[25], &vertices[26], &vertices[37], &vertices[36], &vertices[35]);
    board[3][6]->setVertices(&vertices[26], 0, 0, 0, 0, &vertices[37]);
    board[4][0]->setVertices(0, &vertices[27], &vertices[28], &vertices[38], 0, 0);
    board[4][1]->setVertices(&vertices[28], &vertices[29], &vertices[30], &vertices[40], &vertices[39], &vertices[38]);
    board[4][2]->setVertices(&vertices[30], &vertices[31], &vertices[32], &vertices[42], &vertices[41], &vertices[40]);
    board[4][3]->setVertices(&vertices[32], &vertices[33], &vertices[34], &vertices[44], &vertices[43], &vertices[42]);
    board[4][4]->setVertices(&vertices[34], &vertices[35], &vertices[36], &vertices[46], &vertices[45], &vertices[44]);
    board[4][5]->setVertices(&vertices[36], &vertices[37], 0, 0, 0, &vertices[46]);
    board[5][0]->setVertices(0, &vertices[38], &vertices[39], &vertices[47], 0, 0);
    board[5][1]->setVertices(&vertices[39], &vertices[40], &vertices[41], &vertices[49], &vertices[48], &vertices[47]);
    board[5][2]->setVertices(&vertices[41], &vertices[42], &vertices[43], &vertices[51], &vertices[50], &vertices[49]);
    board[5][3]->setVertices(&vertices[43], &vertices[44], &vertices[45], &vertices[53], &vertices[52], &vertices[51]);
    board[5][4]->setVertices(&vertices[45], &vertices[46], 0, 0, 0, &vertices[53]);
    board[6][0]->setVertices(0, &vertices[47], &vertices[48], 0, 0, 0);
    board[6][1]->setVertices(&vertices[48], &vertices[49], &vertices[50], 0, 0, 0);
    board[6][2]->setVertices(&vertices[50], &vertices[51], &vertices[52], 0, 0, 0);
    board[6][3]->setVertices(&vertices[52], &vertices[53], 0, 0, 0, 0);
}

Hex *GameBoard::getHex_normal(int x, int y)
{
    if (validHex(x, y)) {
        return board[x][y];
    }
    else {
        std::cout << "Incorrect Index: (" << x << ", " << y << ")" << std::endl;
        return board[3][3];
    }
}

Hex *GameBoard::getHex(int x, int y)
{
    int q = y + 3;
    int r = x + 3;

    if (validHex(q, r)) {
        return board[q][r];
    }
    else {
        std::cout << "Incorrect Index: (" << q << ", " << r << ")" << std::endl;
        return board[3][3];
    }
}

bool GameBoard::validHex(int i, int j)
{
    if (i < 7 && i >= 0 && j < 7 && j >= 0)
    {
        if (!((i == 0 && (j == 0 || j == 1 || j == 2)) ||
                        (i == 1 && (j == 0 || j == 1)) ||
                        (i == 2 && j == 0) ||
                        (i == 4 && j == 6) ||
                        (i == 5 && (j == 5 || j == 6)) ||
                        (i == 6 && (j == 4 || j == 5 || j == 6)) ))
                {
                    return true;
                }
                else
                    return false;
    }
    else
        return false;
}

bool GameBoard::notWater(int i, int j)
{
    if (validHex(i, j))
    {
        if (!( (i == 0 && (j == 3 || j == 4 || j == 5 || j == 6)) ||
                    (i == 1 && (j == 2 || j == 6)) ||
                    (i == 2 && (j == 1 || j == 6)) ||
                    (i == 3 && (j == 0 || j == 6)) ||
                    (i == 4 && (j == 0 || j == 5)) ||
                    (i == 5 && (j == 0 || j == 4)) ||
                    (i == 6 && (j == 0 || j == 1 || j == 2 || j == 3)) ))
        {
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

std::vector<MyEnum::Color> GameBoard::placeThief(int x, int y)
{
    std::vector<MyEnum::Color> colors = std::vector<MyEnum::Color>();
    std::vector<Building *> verts;
    Hex *hex;
    if (notWater(y + 3, x + 3))
    {   
        thief->setThief(false);

        hex = getHex(x, y);
        hex->setThief(true);
        thief = hex;

        verts = thief->getVertices();

        for (int i = 0; i < verts.size(); i++)
        {
            if (verts[i]->getColor() != MyEnum::empty && 
                std::find(colors.begin(), colors.end(), verts[i]->getColor()) == colors.end())
            {
                colors.push_back(verts[i]->getColor());
            }
        }
    }
    else
    {
        MyEnum::print("Something is wrong in placeThief()");
    }

    return colors;
}

Hex *GameBoard::getThiefPos()
{
    return thief;
}

void GameBoard::diceRoll(Player playerList[], int roll)
{
    Hex *tempHex;
    std::vector<Hex *> rolledHexes = std::vector<Hex *>();
    std::vector<Building *> hexVerts;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (validHex(i, j))
            {
                tempHex = getHex_normal(i, j);
                if (tempHex->getValue() == roll)
                {
                    rolledHexes.push_back(tempHex);
                }
            }
        }
    }

    for (int i = 0; i < rolledHexes.size(); i++)
    {
        //MyEnum::print(rolledHexes[i]->print());
        rolledHexes[i]->wasRolled(playerList);
    }
}

std::vector<MyEnum::Port> GameBoard::getPlayerPorts(MyEnum::Color color)
{
    //check genPorts
    int genPorts[] = {0, 1, 26, 37, 50, 51, 47, 48};

    std::vector<MyEnum::Port> ret = std::vector<MyEnum::Port>();

    for (int i = 0; i < 8; i++)
    {
        if (vertices[genPorts[i]].getColor() == color)
        {
            ret.push_back(MyEnum::genPort);
            break;
        }
    }

    if (ret.size() == 0)
    {
        ret.push_back(MyEnum::noPort);
    }

    if (vertices[3].getColor() == color || vertices[4].getColor() == color)
    {
        ret.push_back(MyEnum::wheatPort);
    }

    if (vertices[14].getColor() == color || vertices[15].getColor() == color)
    {
        ret.push_back(MyEnum::orePort);
    }

    if (vertices[45].getColor() == color || vertices[46].getColor() == color)
    {
        ret.push_back(MyEnum::woolPort);
    }

    if (vertices[28].getColor() == color || vertices[38].getColor() == color)
    {
        ret.push_back(MyEnum::brickPort);
    }

    if (vertices[7].getColor() == color || vertices[17].getColor() == color)
    {
        ret.push_back(MyEnum::woodPort);
    }

    return ret;
}

//checks everything except, that the building is connected to a player's road
bool GameBoard::validFirstSettlement(int v)
{
    //check out of bounds vert
    if (v < 0 || v > 53)
        return false;
    //check that the vert is empty
    if (!vertices[v].freeSpace())
        return false;
    //get adjacent verts
    std::vector<int> adjacent = getAdjacentVertices(v);

    //sanity check, something is wrong if there aren't any adjacent
    if (adjacent.size() == 0)
        return false;
    //check that all of the adjacent verts are empty
    for (int i = 0; i < adjacent.size(); i++)
    {
        if (!vertices[adjacent[i]].freeSpace())
        {
            return false;
        }
    }

    return true;
}

std::vector<int> GameBoard::possibleSettlementPositions(MyEnum::Color color)
{
    std::vector<int> ret = std::vector<int>();

    for (int i = 0; i < 54; i++)
    {
        if (validBuildingVertex(i, color))
        {
            ret.push_back(i);
        }
    }

    return ret;
}

std::vector<int> GameBoard::possibleCityPositions(MyEnum::Color color)
{
    std::vector<int> ret = std::vector<int>();

    for (int i = 0; i < 54; i++)
    {
        if (validCityVertex(i, color))
        {
            ret.push_back(i);
        }
    }

    return ret;
}

bool GameBoard::validCityVertex(int v, MyEnum::Color color)
{
    if (v < 0 || v > 53)
        return false;

    if (vertices[v].getType() == MyEnum::settlement && vertices[v].getColor() == color)
        return true;

    return false;
}

bool GameBoard::validBuildingVertex(int v, MyEnum::Color color)
{
    //check out of bounds vert
    if (v < 0 || v > 53)
        return false;

    //check that the vert is empty
    if (!vertices[v].freeSpace())
        return false;

    //get adjacent verts
    std::vector<int> adjacent = getAdjacentVertices(v);

    //sanity check
    if (adjacent.size() == 0)
        return false;

    //check that all of the adjacent verts are empty
    for (int i = 0; i < adjacent.size(); i++)
    {
        if (!vertices[adjacent[i]].freeSpace())
        {
            return false;
        }
    }

    //now check that the player has a road to this vert
    for (int i = 0; i < adjacent.size(); i++)
    {
        if (edges[vertsToRoad(v, adjacent[i])].color == color)
        {
            return true;
        }
    }

    return false;
}


std::vector<int> GameBoard::adjacentVerts(bool top, int i, int min, int max, bool doOffset, int offset)
{
    std::vector<int> v;
    if (i - 1 >= min)
        v.push_back(i - 1);
    if (i + 1 <= max)
        v.push_back(i + 1);
    if (doOffset)
    {
        if (top)
            v.push_back(i - offset);
        else
            v.push_back(i + offset);
    }

    return v;
}

std::vector<int> GameBoard::getAdjacentVertices(int vertIndex)
{
    if (vertIndex < 7)
    {
        if (vertIndex % 2)
            return adjacentVerts(true, vertIndex, 0, 6, false, 0);
        else
            return adjacentVerts(false, vertIndex, 0, 6, true, 8);
    }
    else if (vertIndex < 16)
    {
        if (vertIndex % 2)
            return adjacentVerts(false, vertIndex, 7, 15, true, 10);
        else
            return adjacentVerts(true, vertIndex, 7, 15, true, 8);
    }
    else if (vertIndex < 27)
    {
        if (vertIndex % 2)
            return adjacentVerts(true, vertIndex, 16, 26, true, 10);
        else
            return adjacentVerts(false, vertIndex , 16, 26, true, 11);
    }
    else if (vertIndex < 38)
    {
        if (vertIndex % 2)
            return adjacentVerts(true, vertIndex, 27, 37, true, 11);
        else
            return adjacentVerts(false, vertIndex, 27, 37, true, 10);
    }
    else if (vertIndex < 47)
    {
        if (vertIndex % 2)
            return adjacentVerts(false, vertIndex, 38, 46, true, 8);
        else
            return adjacentVerts(true, vertIndex, 38, 46, true, 10);
    }
    else
    {
        if (vertIndex % 2)
            return adjacentVerts(true, vertIndex, 47, 53, true, 8);
        else
            return adjacentVerts(false, vertIndex, 47, 53, false, 0);
    }
}

void GameBoard::buildFirstSettlement(int vertIndex, MyEnum::Color color)
{
    if (validFirstSettlement(vertIndex))
    {
        vertices[vertIndex] = Building(color, MyEnum::settlement, vertices[vertIndex].getPort());
        MyEnum::print("Built a Settlement at position: " + std::to_string(vertIndex));
    }
    else
    {
        MyEnum::print("Invalid building position: " + std::to_string(vertIndex) + ", didn't build the settlement");
    }
}

bool GameBoard::buildASettlement(int vertIndex, MyEnum::Color color)
{
    if (validBuildingVertex(vertIndex, color))
    {
        vertices[vertIndex] = Building(color, MyEnum::settlement, vertices[vertIndex].getPort());
        MyEnum::print("Built a settlement at position: " + std::to_string(vertIndex));
        return true;
    }
    else
    {
        MyEnum::print("Invalid building position: " + std::to_string(vertIndex) + ", didn't build the settlement");
        return false;
    }
}

bool GameBoard::buildACity(int vertIndex, MyEnum::Color color)
{
    if (validCityVertex(vertIndex, color))
    {
        if (vertices[vertIndex].upgrade())
        {
            MyEnum::print("Built a city at position: " + std::to_string(vertIndex));
            return true;
        }
    }
    else
    {
        MyEnum::print("Invalid building position: " + std::to_string(vertIndex) + ", didn't build the city");
        return false;
    }

    return false;
}

std::vector<int> GameBoard::possibleRoadPositions(MyEnum::Color color)
{
    std::vector<int> ret = std::vector<int>();

    for (int i = 0; i < 72; i++)
    {
        if (validBuildingEdge(i, color))
        {
            ret.push_back(i);
        }
    }

    return ret;
}

bool GameBoard::validBuildingEdge(int edgeIndex, MyEnum::Color color)
{
    //check out of bounds and that color isn't empty
    if (edgeIndex < 0 || edgeIndex > 71 || color == MyEnum::empty)
    {
        return false;
    }

    //check that the edge is currently empty
    if (edges[edgeIndex].color != MyEnum::empty)
    {
        return false;
    }

    //get the two vert endpoints of the road
    std::vector<int> verts = roadToVerts(edgeIndex);

    std::vector<int> adjVerts;

    for (int i = 0; i < verts.size(); i++)
    {
        //check the verts at the end of the road
        if (vertices[verts[i]].getColor() == color)
        {
            return true;
        }

        //check the roads touching this road
        adjVerts = getAdjacentVertices(verts[i]);


        for (int j = 0; j < adjVerts.size(); j++)
        {
            if (edges[vertsToRoad(verts[i], adjVerts[j])].getColor() == color)
            {
                return true;
            }
        }
    }

    return false;
}

bool GameBoard::buildARoad(int edgeIndex, MyEnum::Color color)
{
    if (validBuildingEdge(edgeIndex, color))
    {
        edges[edgeIndex] = Road(color);
        MyEnum::print("Built a road at position: " + std::to_string(edgeIndex));
        return true;
    }
    else
    {
        MyEnum::print("Invalid building position: " + std::to_string(edgeIndex) + ", didn't build the road");
        return false;
    }
}

std::vector<int> GameBoard::roadsOffABuilding(int buildingIndex, MyEnum::Color playerColor)
{
    if (buildingIndex < 0 || buildingIndex > 53)
    {
        MyEnum::print("Invalid buildingIndex: " + std::to_string(buildingIndex) + " in roadsOffABuilding");
        return std::vector<int>();
    }

    std::vector<int> adjacentVerts = getAdjacentVertices(buildingIndex);
    std::vector<int> ret = std::vector<int>();
    int roadIndex;

    for (int i = 0; i < adjacentVerts.size(); i++)
    {
        roadIndex = vertsToRoad(buildingIndex, adjacentVerts[i]);
        if (validBuildingEdge(roadIndex, playerColor))
        {
            ret.push_back(roadIndex);
        }
    }

    return ret;
}

int GameBoard::findLongestRoad(Player playerList[], int numPlayers, Player **longestRoad, int longestRoadCount)
{
    MyEnum::Color color;
    int counted[72];
    int tempMax;
    int max = -1;
    int tempPlayer;

    MyEnum::zero(counted, 72);

    for (int i = 0; i < numPlayers; i++)
    {
        color = playerList[i].getColor();
        for (int j = 0; j < 72; j++)
        {
            if (edges[j].getColor() == color)
            {
                tempMax = longestStart(j, counted, color);
                if (tempMax > max)
                {
                    max = tempMax;
                    tempPlayer = i;
                }
                MyEnum::zero(counted, 72);
            }
        }
    }

    if (!*longestRoad && max >= 5)
    {
        *longestRoad = &playerList[tempPlayer];
    }
    else if (*longestRoad && max > longestRoadCount)
    {
        *longestRoad = &playerList[tempPlayer];
    }

    return max;
}

int GameBoard::longestStart(int index, int counted[], MyEnum::Color color)
{
    std::vector<int> verts;

    int vert1, vert2;

    counted[index] = 1;
    verts = roadToVerts(index);

    vert1 = longestVert(verts[0], counted, color);

    vert2 = longestVert(verts[1], counted, color);

    return vert1 + vert2 + 1;
}

int GameBoard::longestEdgeDirected(int index, int counted[], MyEnum::Color color, int previousVert)
{
    std::vector<int> verts;

    counted[index] = 1;
    verts = roadToVerts(index);

    if (verts[0] != previousVert)
    {
        return longestVert(verts[0], counted, color) + 1;
    }
    else
    {
        return longestVert(verts[1], counted, color) + 1;
    }
}

int GameBoard::longestVert(int vert, int counted[], MyEnum::Color color)
{
    std::vector<int> adjacentVerts; 
    int results[3];
    int roadIndex;

    MyEnum::zero(results, 3);

    adjacentVerts = getAdjacentVertices(vert);

    for (int i = 0; i < adjacentVerts.size(); i++)
    {
        roadIndex = vertsToRoad(vert, adjacentVerts[i]);
        if (!counted[roadIndex] && edges[roadIndex].getColor() == color)
        {
            results[i] = longestEdgeDirected(roadIndex, counted, color, vert);
        }
    }

    if (results[0] > results[1])
    {
        if (results[0] > results[2])
        {
            return results[0];
        }
        else
        {
            return results[2];
        }
    }
    else if (results[1] > results[2])
    {
        return results[1];
    }
    else
    {
        return results[2];
    }
}

int GameBoard::getBuildingPoints(MyEnum::Color color)
{
    int points = 0;

    for (int i = 0; i < 53; i++)
    {
        if (vertices[i].getColor() == color)
        {
            if (vertices[i].getType() == MyEnum::city)
            {
                points += 2;
            }
            else
            {
                points++;
            }
        }
    }

    return points;
}

std::vector<MyEnum::Resource> GameBoard::distributeFirstRoundResources(MyEnum::Color color)
{
    std::vector<MyEnum::Resource> ret = std::vector<MyEnum::Resource>();
    std::vector<MyEnum::Resource> resources;

    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].getColor() == color)
        {
            resources = adjacentResource(i);
            for (int j = 0; j < resources.size(); j++)
            {
                ret.push_back(resources[j]);
            }
        }
    }

    return ret;
}

std::vector<MyEnum::Resource> GameBoard::adjacentResource(int vertIndex)
{
    std::vector<MyEnum::Resource> ret = std::vector<MyEnum::Resource>();

    if (vertIndex < 0 || vertIndex > 53)
    {
        MyEnum::print("VertIndex out of bounds: " + std::to_string(vertIndex) + ", in adjacentResource");
        return ret;
    }

    switch(vertIndex)
    {
        case 0:
            ret.push_back(getHex(0, -2)->getResource());
            break;
        case 1:
            ret.push_back(getHex(0, -2)->getResource());
            break;
        case 2:
            ret.push_back(getHex(0, -2)->getResource());
            ret.push_back(getHex(1, -2)->getResource());
            break;
        case 3:
            ret.push_back(getHex(1, -2)->getResource());
            break;
        case 4:
            ret.push_back(getHex(1, -2)->getResource());
            ret.push_back(getHex(2, -2)->getResource());
            break;
        case 5:
            ret.push_back(getHex(2, -2)->getResource());
            break;
        case 6:
            ret.push_back(getHex(2, -2)->getResource());
            break;
        case 7:
            ret.push_back(getHex(-1, -1)->getResource());
            break;
        case 8:
            ret.push_back(getHex(-1, -1)->getResource());
            ret.push_back(getHex(0, -2)->getResource());
            break;
        case 9:
            ret.push_back(getHex(-1, -1)->getResource());
            ret.push_back(getHex(0, -2)->getResource());
            ret.push_back(getHex(0, -1)->getResource());
            break;
        case 10:
            ret.push_back(getHex(0, -2)->getResource());
            ret.push_back(getHex(1, -2)->getResource());
            ret.push_back(getHex(0, -1)->getResource());
            break;
        case 11:
            ret.push_back(getHex(0, -1)->getResource());
            ret.push_back(getHex(1, -1)->getResource());
            ret.push_back(getHex(1, -2)->getResource());
            break;
        case 12:
            ret.push_back(getHex(1, -2)->getResource());
            ret.push_back(getHex(2, -2)->getResource());
            ret.push_back(getHex(1, -1)->getResource());
            break;
        case 13:
            ret.push_back(getHex(1, -1)->getResource());
            ret.push_back(getHex(2, -1)->getResource());
            ret.push_back(getHex(2, -2)->getResource());
            break;
        case 14:
            ret.push_back(getHex(2, -2)->getResource());
            ret.push_back(getHex(2, -1)->getResource());
            break;
        case 15:
            ret.push_back(getHex(2, -1)->getResource());
            break;
        case 16:   
            ret.push_back(getHex(-2, 0)->getResource());
            break;
        case 17:
            ret.push_back(getHex(-2, 0)->getResource());
            ret.push_back(getHex(-1, -1)->getResource());
            break;
        case 18:
            ret.push_back(getHex(-2, 0)->getResource());
            ret.push_back(getHex(-1, 0)->getResource());
            ret.push_back(getHex(-1, -1)->getResource());
            break;
        case 19:
            ret.push_back(getHex(-1, -1)->getResource());
            ret.push_back(getHex(0, -1)->getResource());
            ret.push_back(getHex(-1, 0)->getResource());
            break;
        case 20:
            ret.push_back(getHex(0, -1)->getResource());
            ret.push_back(getHex(-1, 0)->getResource());
            break;
        case 21:
            ret.push_back(getHex(0, -1)->getResource());
            ret.push_back(getHex(1, -1)->getResource());
            break;
        case 22:
            ret.push_back(getHex(1, -1)->getResource());
            ret.push_back(getHex(1, 0)->getResource());
            break;
        case 23:
            ret.push_back(getHex(1, -1)->getResource());
            ret.push_back(getHex(2, -1)->getResource());
            ret.push_back(getHex(1, 0)->getResource());
            break;
        case 24:
            ret.push_back(getHex(2, -1)->getResource());
            ret.push_back(getHex(1, 0)->getResource());
            ret.push_back(getHex(2, 0)->getResource());
            break;
        case 25:
            ret.push_back(getHex(2, -1)->getResource());
            ret.push_back(getHex(2, 0)->getResource());
            break;
        case 26:
            ret.push_back(getHex(2, 0)->getResource());
            break;
        case 27:
            ret.push_back(getHex(-2, 0)->getResource());
            break;
        case 28:
            ret.push_back(getHex(-2, 0)->getResource());
            ret.push_back(getHex(-2, 1)->getResource());
            break;
        case 29:
            ret.push_back(getHex(-2, 0)->getResource());
            ret.push_back(getHex(-1, 0)->getResource());
            ret.push_back(getHex(-2, 1)->getResource());
            break;
        case 30:
            ret.push_back(getHex(-1, 0)->getResource());
            ret.push_back(getHex(-2, 1)->getResource());
            ret.push_back(getHex(-1, 1)->getResource());
            break;
        case 31:
            ret.push_back(getHex(-1, 0)->getResource());
            ret.push_back(getHex(-1, 1)->getResource());
            break;
        case 32:
            ret.push_back(getHex(-1, 1)->getResource());
            ret.push_back(getHex(0, 1)->getResource());
            break;
        case 33:
            ret.push_back(getHex(0, 1)->getResource());
            ret.push_back(getHex(1, 0)->getResource());
            break;
        case 34:
            ret.push_back(getHex(1, 0)->getResource());
            ret.push_back(getHex(0, 1)->getResource());
            ret.push_back(getHex(1, 1)->getResource());
            break;
        case 35:
            ret.push_back(getHex(1, 0)->getResource());
            ret.push_back(getHex(2, 0)->getResource());
            ret.push_back(getHex(1, 1)->getResource());
            break;
        case 36:
            ret.push_back(getHex(2, 0)->getResource());
            ret.push_back(getHex(1, 1)->getResource());
            break;
        case 37:
            ret.push_back(getHex(2, 0)->getResource());
            break;
        case 38:
            ret.push_back(getHex(-2, 1)->getResource());
            break;
        case 39:
            ret.push_back(getHex(-2, 1)->getResource());
            ret.push_back(getHex(-2, 2)->getResource());
            break;
        case 40:
            ret.push_back(getHex(-2, 1)->getResource());
            ret.push_back(getHex(-1, 1)->getResource());
            ret.push_back(getHex(-2, 2)->getResource());
            break;
        case 41:
            ret.push_back(getHex(-1, 1)->getResource());
            ret.push_back(getHex(-2, 2)->getResource());
            ret.push_back(getHex(-1, 2)->getResource());
            break;
        case 42:
            ret.push_back(getHex(-1, 1)->getResource());
            ret.push_back(getHex(0, 1)->getResource());
            ret.push_back(getHex(-1, 2)->getResource());
            break;
        case 43:
            ret.push_back(getHex(0, 1)->getResource());
            ret.push_back(getHex(-1, 2)->getResource());
            ret.push_back(getHex(0, 2)->getResource());
            break;
        case 44:
            ret.push_back(getHex(0, 1)->getResource());
            ret.push_back(getHex(1, 1)->getResource());
            ret.push_back(getHex(0, 2)->getResource());
            break;
        case 45:
            ret.push_back(getHex(1, 1)->getResource());
            ret.push_back(getHex(0, 2)->getResource());
            break;
        case 46:
            ret.push_back(getHex(1, 1)->getResource());
            break;
        case 47:
            ret.push_back(getHex(-2, 2)->getResource());
            break;
        case 48:
            ret.push_back(getHex(-2, 2)->getResource());
            break;
        case 49:
            ret.push_back(getHex(-2, 2)->getResource());
            ret.push_back(getHex(-1, 2)->getResource());
            break;
        case 50:
            ret.push_back(getHex(-1, 2)->getResource());
            break;
        case 51:
            ret.push_back(getHex(-1, 2)->getResource());
            ret.push_back(getHex(0, 2)->getResource());
            break;
        case 52:
            ret.push_back(getHex(0, 2)->getResource());
            break;
        case 53:
            ret.push_back(getHex(0, 2)->getResource());
            break;
    }
    

    return ret;

}

std::vector<int> GameBoard::makeVector_3(int x, int y, int z)
{
    int arr[] = {x, y, z};
    return std::vector<int>(arr, arr + sizeof(arr) / sizeof(int));
}

std::vector<int> GameBoard::makeVector_2(int x, int y)
{
    int arr[] = {x, y};
    return std::vector<int>(arr, arr + sizeof(arr) / sizeof(int));
}

std::vector<int> GameBoard::roadToVerts(int roadIndex)
{
    if (roadIndex < 0 || roadIndex > 71)
        return std::vector<int>();

    switch(roadIndex)
    {
        case 0: 
            return makeVector_2(0, 1);
        case 1: 
            return makeVector_2(1, 2);
        case 2: 
            return makeVector_2(2, 3);
        case 3: 
            return makeVector_2(3, 4);
        case 4: 
            return makeVector_2(4, 5);
        case 5: 
            return makeVector_2(5, 6);
        case 6: 
            return makeVector_2(0, 8);
        case 7: 
            return makeVector_2(2, 10);
        case 8: 
            return makeVector_2(4, 12);
        case 9: 
            return makeVector_2(6, 14);
        case 10: 
            return makeVector_2(7, 8);
        case 11: 
            return makeVector_2(8, 9);
        case 12: 
            return makeVector_2(9, 10);
        case 13: 
            return makeVector_2(10, 11);
        case 14: 
            return makeVector_2(11, 12);
        case 15: 
            return makeVector_2(12, 13);
        case 16: 
            return makeVector_2(13, 14);
        case 17: 
            return makeVector_2(14, 15);
        case 18: 
            return makeVector_2(7, 17);
        case 19: 
            return makeVector_2(9, 19);
        case 20: 
            return makeVector_2(11, 21);
        case 21: 
            return makeVector_2(13, 23);
        case 22: 
            return makeVector_2(15, 25);
        case 23: 
            return makeVector_2(16, 17);
        case 24: 
            return makeVector_2(17, 18);
        case 25: 
            return makeVector_2(18, 19);
        case 26: 
            return makeVector_2(19, 20);
        case 27: 
            return makeVector_2(20, 21);
        case 28: 
            return makeVector_2(21, 22);
        case 29: 
            return makeVector_2(22, 23);
        case 30: 
            return makeVector_2(23, 24);
        case 31: 
            return makeVector_2(24, 25);
        case 32: 
            return makeVector_2(25, 26);       
        case 33: 
            return makeVector_2(16, 27);
        case 34: 
            return makeVector_2(18, 29);
        case 35: 
            return makeVector_2(20, 31);
        case 36: 
            return makeVector_2(22, 33);
        case 37: 
            return makeVector_2(24, 35);
        case 38: 
            return makeVector_2(26, 37);
        case 39: 
            return makeVector_2(27, 28);
        case 40: 
            return makeVector_2(28, 29);
        case 41: 
            return makeVector_2(29, 30);
        case 42: 
            return makeVector_2(30, 31);
        case 43: 
            return makeVector_2(31, 32);
        case 44: 
            return makeVector_2(32, 33);  
        case 45: 
            return makeVector_2(33, 34);
        case 46: 
            return makeVector_2(34, 35);
        case 47: 
            return makeVector_2(35, 36);
        case 48: 
            return makeVector_2(36, 37);
        case 49: 
            return makeVector_2(28, 38);
        case 50: 
            return makeVector_2(30, 40);
        case 51: 
            return makeVector_2(32, 42);
        case 52: 
            return makeVector_2(34, 44);
        case 53: 
            return makeVector_2(36, 46);
        case 54: 
            return makeVector_2(38, 39);
        case 55: 
            return makeVector_2(39, 40);
        case 56: 
            return makeVector_2(40, 41);
        case 57: 
            return makeVector_2(41, 42);
        case 58: 
            return makeVector_2(42, 43);
        case 59: 
            return makeVector_2(43, 44);
        case 60: 
            return makeVector_2(44, 45);
        case 61: 
            return makeVector_2(45, 46);
        case 62: 
            return makeVector_2(39, 47);
        case 63: 
            return makeVector_2(41, 49);
        case 64: 
            return makeVector_2(43, 51);
        case 65: 
            return makeVector_2(45, 53);   
        case 66: 
            return makeVector_2(47, 48);
        case 67: 
            return makeVector_2(48, 49);
        case 68: 
            return makeVector_2(49, 50);
        case 69: 
            return makeVector_2(50, 51);
        case 70: 
            return makeVector_2(51, 52);
        case 71: 
            return makeVector_2(52, 53);
    }

    return std::vector<int>();
}

int GameBoard::vertsToRoad(int vertIndex1, int vertIndex2)
{
    if (vertIndex1 < 0 || vertIndex1 > 53 || vertIndex2 < 0 || vertIndex2 > 53 || vertIndex1 == vertIndex2)
        return -1;

    if (vertIndex1 >= 0 && vertIndex1 <= 6 && vertIndex2 >= 0 && vertIndex2 <= 6)
    {
        if (MyEnum::roadVerts(vertIndex1, vertIndex2, 0, 1))
        {
            return 0;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 1, 2))
        {
            return 1;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 2, 3))
        {
            return 2;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 3, 4))
        {
            return 3;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 4, 5))
        {
            return 4;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 5, 6))
        {
            return 5;
        }
    }

    if (MyEnum::roadVerts(vertIndex1, vertIndex2, 0, 8))
    {
        return 6;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 2, 10))
    {
        return 7;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 4, 12))
    {
        return 8;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 6, 14))
    {
        return 9;
    }

    if (vertIndex1 >= 7 && vertIndex1 <= 15 && vertIndex2 >= 7 && vertIndex2 <= 15)
    {
        if (MyEnum::roadVerts(vertIndex1, vertIndex2, 7, 8))
        {
            return 10;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 8, 9))
        {
            return 11;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 9, 10))
        {
            return 12;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 10, 11))
        {
            return 13;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 11, 12))
        {
            return 14;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 12, 13))
        {
            return 15;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 13, 14))
        {
            return 16;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 14, 15))
        {
            return 17;
        }
    }

    if (MyEnum::roadVerts(vertIndex1, vertIndex2, 7, 17))
    {
        return 18;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 9, 19))
    {
        return 19;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 11, 21))
    {
        return 20;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 13, 23))
    {
        return 21;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 15, 25))
    {
        return 22;
    }

    if (vertIndex1 >= 16 && vertIndex1 <= 26 && vertIndex2 >= 16 && vertIndex2 <= 26)
    {
        if (MyEnum::roadVerts(vertIndex1, vertIndex2, 16, 17))
        {
            return 23;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 17, 18))
        {
            return 24;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 18, 19))
        {
            return 25;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 19, 20))
        {
            return 26;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 20, 21))
        {
            return 27;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 21, 22))
        {
            return 28;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 22, 23))
        {
            return 29;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 23, 24))
        {
            return 30;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 24, 25))
        {
            return 31;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 25, 26))
        {
            return 32;
        }
    }

    if (MyEnum::roadVerts(vertIndex1, vertIndex2, 16, 27))
    {
        return 33;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 18, 29))
    {
        return 34;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 20, 31))
    {
        return 35;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 22, 33))
    {
        return 36;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 24, 35))
    {
        return 37;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 26, 37))
    {
        return 38;
    }

    if (vertIndex1 >= 27 && vertIndex1 <= 37 && vertIndex2 >= 27 && vertIndex2 <= 37)
    {
        if (MyEnum::roadVerts(vertIndex1, vertIndex2, 27, 28))
        {
            return 39;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 28, 29))
        {
            return 40;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 29, 30))
        {
            return 41;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 30, 31))
        {
            return 42;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 31, 32))
        {
            return 43;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 32, 33))
        {
            return 44;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 33, 34))
        {
            return 45;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 34, 35))
        {
            return 46;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 35, 36))
        {
            return 47;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 36, 37))
        {
            return 48;
        }
    }

    if (MyEnum::roadVerts(vertIndex1, vertIndex2, 28, 38))
    {
        return 49;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 30, 40))
    {
        return 50;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 32, 42))
    {
        return 51;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 34, 44))
    {
        return 52;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 36, 46))
    {
        return 53;
    }

    if (vertIndex1 >= 38 && vertIndex1 <= 46 && vertIndex2 >= 38 && vertIndex2 <= 46)
    {
        if (MyEnum::roadVerts(vertIndex1, vertIndex2, 38, 39))
        {
            return 54;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 39, 40))
        {
            return 55;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 40, 41))
        {
            return 56;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 41, 42))
        {
            return 57;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 42, 43))
        {
            return 58;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 43, 44))
        {
            return 59;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 44, 45))
        {
            return 60;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 45, 46))
        {
            return 61;
        }
    }
    
    if (MyEnum::roadVerts(vertIndex1, vertIndex2, 39, 47))
    {
        return 62;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 41, 49))
    {
        return 63;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 43, 51))
    {
        return 64;
    }
    else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 45, 53))
    {
        return 65;
    }

    if (vertIndex1 >= 47 && vertIndex1 <= 53 && vertIndex2 >= 47 && vertIndex2 <= 53)
    {
        if (MyEnum::roadVerts(vertIndex1, vertIndex2, 47, 48))
        {
            return 66;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 48, 49))
        {
            return 67;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 49, 50))
        {
            return 68;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 50, 51))
        {
            return 69;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 51, 52))
        {
            return 70;
        }
        else if (MyEnum::roadVerts(vertIndex1, vertIndex2, 52, 53))
        {
            return 71;
        }
    }

    return -1;
}


void GameBoard::defaultResources()
{
    resourceList[0] = MyEnum::water;
    resourceList[1] = MyEnum::water;
    resourceList[2] = MyEnum::water;
    resourceList[3] = MyEnum::water;
    resourceList[4] = MyEnum::water;
    //top row
    resourceList[5] = MyEnum::ore;
    resourceList[6] = MyEnum::wool;
    resourceList[7] = MyEnum::wood;
    //water
    resourceList[8] = MyEnum::water;
    resourceList[9] = MyEnum::water;
    //second top
    resourceList[10] = MyEnum::wheat;
    resourceList[11] = MyEnum::brick;
    resourceList[12] = MyEnum::wool;
    resourceList[13] = MyEnum::brick;
    //water
    resourceList[14] = MyEnum::water;
    resourceList[15] = MyEnum::water;
    //mid
    resourceList[16] = MyEnum::wheat;
    resourceList[17] = MyEnum::wood;
    resourceList[18] = MyEnum::desert;
    resourceList[19] = MyEnum::wood;
    resourceList[20] = MyEnum::ore;
    //water
    resourceList[21] = MyEnum::water;
    resourceList[22] = MyEnum::water;
    //second bot
    resourceList[23] = MyEnum::wood;
    resourceList[24] = MyEnum::ore;
    resourceList[25] = MyEnum::wheat;
    resourceList[26] = MyEnum::wool;
    //water
    resourceList[27] = MyEnum::water;
    resourceList[28] = MyEnum::water;
    //bot row
    resourceList[29] = MyEnum::brick;
    resourceList[30] = MyEnum::wheat;
    resourceList[31] = MyEnum::wool;

    resourceList[32] = MyEnum::water;
    resourceList[33] = MyEnum::water;
    resourceList[34] = MyEnum::water;
    resourceList[35] = MyEnum::water;
    resourceList[36] = MyEnum::water;
}

void GameBoard::defaultValues()
{
    valueList[0] = -1;
    valueList[1] = -1;
    valueList[2] = -1;
    valueList[3] = -1;
    valueList[4] = -1;
    //top row
    valueList[5] = 10;
    valueList[6] = 2;
    valueList[7] = 9;
    //water
    valueList[8] = -1;
    valueList[9] = -1;
    //second top
    valueList[10] = 12;
    valueList[11] = 6;
    valueList[12] = 4;
    valueList[13] = 10;
    //water
    valueList[14] = -1;
    valueList[15] = -1;
    //mid
    valueList[16] = 9;
    valueList[17] = 11;
    valueList[18] = -1;
    valueList[19] = 3;
    valueList[20] = 8;
    //water
    valueList[21] = -1;
    valueList[22] = -1;
    //second bot
    valueList[23] = 8;
    valueList[24] = 3;
    valueList[25] = 4;
    valueList[26] = 5;
    //water
    valueList[27] = -1;
    valueList[28] = -1;
    //bot row
    valueList[29] = 5;
    valueList[30] = 6;
    valueList[31] = 11;

    valueList[32] = -1;
    valueList[33] = -1;
    valueList[34] = -1;
    valueList[35] = -1;
    valueList[36] = -1;
}

void GameBoard::defaultPorts()
{
    vertices[0].setPort(MyEnum::genPort);
    vertices[1].setPort(MyEnum::genPort);

    vertices[3].setPort(MyEnum::wheatPort);
    vertices[4].setPort(MyEnum::wheatPort);

    vertices[14].setPort(MyEnum::orePort);
    vertices[15].setPort(MyEnum::orePort);

    vertices[26].setPort(MyEnum::genPort);
    vertices[37].setPort(MyEnum::genPort);

    vertices[45].setPort(MyEnum::woolPort);
    vertices[46].setPort(MyEnum::woolPort);

    vertices[50].setPort(MyEnum::genPort);
    vertices[51].setPort(MyEnum::genPort);

    vertices[47].setPort(MyEnum::genPort);
    vertices[48].setPort(MyEnum::genPort);

    vertices[28].setPort(MyEnum::brickPort);
    vertices[38].setPort(MyEnum::brickPort);

    vertices[7].setPort(MyEnum::woodPort);
    vertices[17].setPort(MyEnum::woodPort);
}

glm::vec2 GameBoard::intToHex(int i)
{
    switch(i)
    {
        case 0:
            return glm::vec2(0, -3);
        case 1:
            return glm::vec2(1, -3);
        case 2:
            return glm::vec2(2, -3);
        case 3:
            return glm::vec2(3, -3);
        case 4:
            return glm::vec2(-1, -2);
        case 5:
            return glm::vec2(0, -2);
        case 6:
            return glm::vec2(1, -2);
        case 7:
            return glm::vec2(2, -2);
        case 8:
            return glm::vec2(3, -2);
        case 9:
            return glm::vec2(-2, -1);
        case 10:
            return glm::vec2(-1, -1);
        case 11:
            return glm::vec2(0, -1);
        case 12:
            return glm::vec2(1, -1);
        case 13:
            return glm::vec2(2, -1);
        case 14:
            return glm::vec2(3, -1);
        case 15:
            return glm::vec2(-3, 0);
        case 16:
            return glm::vec2(-2, 0);
        case 17:
            return glm::vec2(-1, 0);
        case 18:
            return glm::vec2(0, 0);
        case 19:
            return glm::vec2(1, 0);
        case 20:
            return glm::vec2(2, 0);
        case 21:
            return glm::vec2(3, 0);
        case 22:
            return glm::vec2(-3, 1);
        case 23:
            return glm::vec2(-2, 1);
        case 24:
            return glm::vec2(-1, 1);
        case 25:
            return glm::vec2(0, 1);
        case 26:
            return glm::vec2(1, 1);
        case 27:
            return glm::vec2(2, 1);
        case 28:
            return glm::vec2(-3, 2);
        case 29:
            return glm::vec2(-2, 2);
        case 30:
            return glm::vec2(-1, 2);
        case 31:
            return glm::vec2(0, 2);
        case 32:
            return glm::vec2(1, 2);
        case 33:
            return glm::vec2(-3, 3);
        case 34:
            return glm::vec2(-2, 3);
        case 35:
            return glm::vec2(-1, 3);
        case 36:
            return glm::vec2(0, 3);
    }

    MyEnum::print("Something wrong in intToHex");
    return glm::vec2(9,9);
}








