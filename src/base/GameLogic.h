
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include "GameBoard.h"
#include "Player.h"
#include "MyEnum.h"

using namespace std;

class GameLogic {

public:
	static void printHex(GameBoard *board);
	static void printPlayers(Player playerList[], int numPlayers);
	static int nextPlayer(int curIndex, int numPlayers);
	static void placeSettlementAndRoad(Player cur, GameBoard *board);
	static void roll(Player playList[], GameBoard *board, int numPlayers, int curPlayer);
	static void build(Player *player, GameBoard *board, std::vector<MyEnum::DevCard> &developmentCards);
	static void trade(Player playerList[], int numPlayers, int curPlayer, GameBoard *board);
	static void portTrade(Player playerList[], int curPlayer, GameBoard *board);
	static Player *playerFromColor(MyEnum::Color color, Player playerList[], int numPlayers);
	static void placeThief(Player playerList[], GameBoard *board, int numPlayers, int curPlayer);
	static void playDevCard(Player playList[], GameBoard *board, int numPlayers, int curPlayer);
	static void playAKnight(Player playerList[], GameBoard *board, int numPlayers, int curPlayer);
	static void buildARoad_DevCard(MyEnum::Color color, GameBoard *board);
	static void playARoadBuilding(Player playerList[], GameBoard *board, int curPlayer);
	static void playAYearOfPlenty(Player playerList[], int curPlayer);
	static void playAMonopoly(Player playerList[], int numPlayers, int curPlayer, MyEnum::Resource type);
	static void initDevCards(std::vector<MyEnum::DevCard> &deck);
	static void checkLargestArmy(Player playerList[], int numPlayers, Player **largestArmy);
	static int checkLongestRoad(Player playerList[], GameBoard *board, int numPlayers, Player **longestRoad, int longestRoadCount);
	static void pointsForAllPlayers(Player playerList[], GameBoard *board, int numPlayers, Player *largestArmy, Player *longestRoad);
	static int pointsForPlayer(GameBoard *board, MyEnum::Color color, Player *largestArmy, Player *longestRoad);
	static bool checkWinner(Player playerList[], GameBoard *board, int numPlayers, Player *largestArmy, Player *longestRoad);

};

#endif