
#include "GameLogic.h"

void GameLogic::printHex(GameBoard *board)
{
	int x, y;

	std::cout << "Choose a Hex" << std::endl;
	std::cin >> x;
	std::cin >> y;

	std::cout << board->getHex(x, y)->print() << std::endl;

}

int GameLogic::nextPlayer(int curIndex, int numPlayers)
{
	return ++curIndex % numPlayers;
}

//place first and second settlement and road
void GameLogic::placeSettlementAndRoad(Player cur, GameBoard *board)
{
	int settlementVert;
	int roadEdge;
	bool goodVert;
	bool goodEdge;

	MyEnum::print(cur.getColorString() + " place a Settlement and then Road.");
	do
	{
		cin >> settlementVert;
		goodVert = board->validFirstSettlement(settlementVert);

		if (settlementVert < 0 || settlementVert > 53)
			MyEnum::print("Vertex out of bounds.");
		else if (!goodVert)
			MyEnum::print("Invalid Building spot");
	}
	while(!goodVert);

	board->buildFirstSettlement(settlementVert, cur.getColor());

	MyEnum::print("Choose a road off of your settlement.");
	std::vector<int> branchRoads = board->roadsOffABuilding(settlementVert, cur.getColor());
	MyEnum::print(MyEnum::vectorToString(branchRoads));
	do
	{
		cin >> roadEdge;

		goodEdge = board->validBuildingEdge(roadEdge, cur.getColor());

		if (roadEdge < 0 || roadEdge > 71)
			MyEnum::print("Edge out of bounds");
		else if (!goodEdge)
			MyEnum::print("Invalid Road spot");

		if (std::find(branchRoads.begin(), branchRoads.end(), roadEdge) == branchRoads.end() && goodEdge)
		{
			goodEdge = false;
			MyEnum::print("Must choose a road connected to the settlement you just placed");
		}

	}
	while(!goodEdge);

	board->buildARoad(roadEdge, cur.getColor());

}

void GameLogic::build(Player *player, GameBoard *board, std::vector<MyEnum::DevCard> &developmentCards)
{
	std::vector<std::string> buildings;
	std::vector<int> possibleVerts;
	std::vector<int> possibleEdges;
	int buildThis = -1;

	buildings = player->canBuild();

	MyEnum::print(MyEnum::colorToString(player->getColor()) + " can build:\n" + MyEnum::vectorToString(buildings));

	if (buildings.size() > 0)
	{
		while (buildThis < 0 || buildThis >= buildings.size())
		{
			MyEnum::print("Build: ");
			cin >> buildThis;
		}

		if (buildings[buildThis] == "settlement")
		{
			possibleVerts = board->possibleSettlementPositions(player->getColor());

			if (possibleVerts.size() == 0)
			{
				MyEnum::print("No valid posistions for a settlement");
			}
			else
			{
				MyEnum::print(possibleVerts);
				buildThis = -1;
				while (buildThis < 0 || buildThis >= possibleVerts.size())
				{
					MyEnum::print("Build Here:");
					cin >> buildThis;
				}

				if (!board->buildASettlement(possibleVerts[buildThis], player->getColor()))
				{
					MyEnum::print("Something wrong with buildASettlement(GameBoard) at vert: " + std::to_string(possibleVerts[buildThis]) + " and color: " + MyEnum::colorToString(player->getColor()));
				}

				if (!player->buildSettlement())
				{
					MyEnum::print("Something wrong with buildSettlement(Player) at vert: " + std::to_string(possibleVerts[buildThis]) + " and color: " + MyEnum::colorToString(player->getColor()));
				}
			}
		}
		else if (buildings[buildThis] == "road")
		{
			possibleEdges = board->possibleRoadPositions(player->getColor());
			if (possibleEdges.size() == 0)
			{
				MyEnum::print("No valid positions for a road");
			}
			else
			{
				MyEnum::print(possibleEdges);
				buildThis = -1;
				while (buildThis < 0 || buildThis >= possibleEdges.size())
				{
					MyEnum::print("Build Here");
					cin >> buildThis;
				}

				if (!board->buildARoad(possibleEdges[buildThis], player->getColor()))
				{
					MyEnum::print("Something wrong with buildARoad(GameBoard) at edge: " + std::to_string(possibleEdges[buildThis]) + " and color: " + MyEnum::colorToString(player->getColor()));
				}

				if (!player->buildRoad())
				{
					MyEnum::print("Something wrong with buildRoad(Player) at edge: " + std::to_string(possibleEdges[buildThis]) + " and color: " + MyEnum::colorToString(player->getColor()));	
				}
			}
		}
		else if (buildings[buildThis] == "city")
		{
			possibleVerts = board->possibleCityPositions(player->getColor());

			if (possibleVerts.size() == 0)
			{
				MyEnum::print("No valid positions for a city");
			}
			else
			{
				MyEnum::print(possibleVerts);
				buildThis = -1;
				while (buildThis < 0 || buildThis >= possibleVerts.size())
				{
					MyEnum::print("Build Here:");
					cin >> buildThis;
				}

				if (!board->buildACity(possibleVerts[buildThis], player->getColor()))
				{
					MyEnum::print("Something wrong with buildACity(GameBoard) at vert " + std::to_string(possibleVerts[buildThis]) + " and color: " + MyEnum::colorToString(player->getColor()));
				}

				if (!player->buildCity())
				{
					MyEnum::print("Something wrong with buildCity(Player) at vert: " + std::to_string(possibleVerts[buildThis]) + " and color: " + MyEnum::colorToString(player->getColor()));	
				}
			}
		}
		else if (buildings[buildThis] == "dev card")
		{
			if (!player->buildDevelopmentCard())
			{
				MyEnum::print("Something went wrong in buildDevelopmentCard()");
			}
			else
			{
				int random = rand() % developmentCards.size();
				player->gainDevelopmentCard(developmentCards[random]);
				developmentCards.erase(developmentCards.begin() + random);
			}
		}
		
	}
	
}

Player * GameLogic::playerFromColor(MyEnum::Color color, Player playerList[], int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)
	{
		if (playerList[i].getColor() == color)
		{
			return &playerList[i];
		}
	}

	return 0;
}

void GameLogic::placeThief(Player playerList[], GameBoard *board, int numPlayers, int curPlayer)
{
	int x = 10, y = 10;
	MyEnum::print(MyEnum::colorToString(playerList[curPlayer].getColor()) + " place the theif in a new posistion.");
	std::vector<MyEnum::Color> playersToStealFrom = std::vector<MyEnum::Color>();
	std::vector<MyEnum::Resource> giveTo = std::vector<MyEnum::Resource>(1);
	int choice = -1;
	int handSize;
	Player *removeFromP;
	MyEnum::Resource card;

	
	while (!board->notWater(y + 3, x + 3) || board->getThiefPos() == board->getHex(x, y))
	{
		x = 10;
		y = 10;
		while (x < -3 || x > 3)
		{
			MyEnum::print("X:");
			std::cin >> x;
		}

		while (y < -3 || y > 3)
		{
			MyEnum::print("Y:");
			std::cin >> y;
		}
	} 

	playersToStealFrom = board->placeThief(x, y);

	for (int i = 0; i < playersToStealFrom.size(); i++)
	{
		if (playersToStealFrom[i] == playerList[curPlayer].getColor())
		{
			playersToStealFrom.erase(playersToStealFrom.begin() + i);
		}
	}

	if (playersToStealFrom.size() > 0)
	{
		MyEnum::print("Steal from:");
		MyEnum::print(MyEnum::vectorToString(playersToStealFrom));

		while (choice < 0 || choice > playersToStealFrom.size() - 1)
		{
			cin >> choice;
		}

		removeFromP = playerFromColor(playersToStealFrom[choice], playerList, numPlayers);
		handSize = removeFromP->getHand().handSize();

		if (handSize > 0)
		{
			MyEnum::print("Pick a card (0 - " + std::to_string(handSize - 1) + ")");

			choice = -1;
			while (choice < 0 || choice > handSize - 1)
			{
				cin >> choice;
			}

			card = removeFromP->removeRandomCard();
			if (card != MyEnum::water)
			{
				giveTo[0] = card;
				playerList[curPlayer].gainResourceCards(giveTo);
			}
			else
			{
				MyEnum::print("Something went wrong with removeRandomCard()");
			}
		}
		else
		{
			MyEnum::print("Picked player has no cards to steal");
		}

	}
	else
	{
		MyEnum::print("No one to steal from");
	}
}


void GameLogic::printPlayers(Player playerList[], int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)
	{
		MyEnum::print(playerList[i].print() + "\n");
	}
}

void GameLogic::roll(Player playList[], GameBoard *board, int numPlayers, int curPlayer)
{
	int roll = (rand() % 6) + 1 + (rand() % 6) + 1;

	MyEnum::print("A " + std::to_string(roll) + " was rolled");

	if (roll == 7)
	{
		for (int i = 0; i < numPlayers; i++)
		{
			playList[i].rolledASeven();
		}
		placeThief(playList, board, numPlayers, curPlayer);
	}
	else
	{
		board->diceRoll(playList, roll);
	}

	printPlayers(playList, 2);
}

void GameLogic::portTrade(Player playerList[], int curPlayer, GameBoard *board)
{
	std::vector<MyEnum::Port> options;
	int portChoice = -1;

	options = board->getPlayerPorts(playerList[curPlayer].getColor());

	while (portChoice < 0 || portChoice > options.size() - 1)
	{
		MyEnum::print(MyEnum::vectorToString(options));
		cin >> portChoice;
	}

	playerList[curPlayer].portTrade(options[portChoice]);
}

void GameLogic::trade(Player playerList[], int numPlayers, int curPlayer, GameBoard *board)
{
	int tradeWith = -999;
	std::string tradeTake = "";
	std::string tradeGive = "";
	std::string answer = "";
	std::vector<MyEnum::Resource> take = std::vector<MyEnum::Resource>();
	std::vector<MyEnum::Resource> give = std::vector<MyEnum::Resource>();

	std::string players = "";

	for (int i = 0; i < numPlayers; i++)
	{
		players += MyEnum::colorToString(playerList[i].getColor()) + ", ";
	}


	while (tradeWith < -1 || tradeWith > numPlayers || tradeWith == curPlayer)
	{
		MyEnum::print("Trade with which player Port Trade(" + std::to_string(numPlayers) + "), " + players);
		cin >> tradeWith;
	}

	if (tradeWith == numPlayers)
	{
		portTrade(playerList, curPlayer, board);
	}
	else
	{

		MyEnum::print("I want:");

		while (tradeTake != "done")
		{
			std::getline(std::cin, tradeTake);
			
			if (tradeTake == "wood")
				take.push_back(MyEnum::wood);
			else if (tradeTake == "brick")
				take.push_back(MyEnum::brick);
			else if (tradeTake == "wheat")
				take.push_back(MyEnum::wheat);
			else if (tradeTake == "wool")
				take.push_back(MyEnum::wool);
			else if (tradeTake == "ore")
				take.push_back(MyEnum::ore);
		}

		MyEnum::print("Will give you");

		while (tradeGive != "done")
		{
			std::getline(std::cin, tradeGive);

			if (tradeGive == "wood")
				give.push_back(MyEnum::wood);
			else if (tradeGive == "brick")
				give.push_back(MyEnum::brick);
			else if (tradeGive == "wheat")
				give.push_back(MyEnum::wheat);
			else if (tradeGive == "wool")
				give.push_back(MyEnum::wool);
			else if (tradeGive == "ore")
				give.push_back(MyEnum::ore);
		}

		MyEnum::print(MyEnum::colorToString(playerList[tradeWith].getColor()) + " will you accept the trade:");
		MyEnum::print(MyEnum::vectorToString(give) + " for:");
		MyEnum::print(MyEnum::vectorToString(take));

		while (answer != "yes" && answer != "no")
		{
			std::getline(std::cin, answer);
		}

		if (answer == "yes")
		{
			if (playerList[curPlayer].canGiveResources(give) && playerList[tradeWith].canGiveResources(take))
			{
				playerList[curPlayer].gainResourceCards(take);
				playerList[curPlayer].removeResourceCards(give);

				playerList[tradeWith].gainResourceCards(give);
				playerList[tradeWith].removeResourceCards(take);

				MyEnum::print("Trade went through");
				MyEnum::print(playerList[curPlayer].print());
				MyEnum::print(playerList[tradeWith].print());
			}
			else
			{
				MyEnum::print("Invalid Trade");
			}
		}
		else
		{
			MyEnum::print("Trade Declined");
		}
	}

}

void GameLogic::playDevCard(Player playList[], GameBoard *board, int numPlayers, int curPlayer)
{
	std::string choice = "";

	MyEnum::print("Which one");
	MyEnum::print(playList[curPlayer].getHand().printDevelopmentCards());

	while (choice != "knight" && choice != "road" && choice != "year" && choice != "monopoly" && choice != "cancel")
	{
		std::getline(std::cin, choice);
	}

	if (choice == "knight")
	{
		playAKnight(playList, board, numPlayers, curPlayer);
	}
	else if (choice == "road")
	{
		playARoadBuilding(playList, board, curPlayer);
	}
	else if (choice == "year")
	{
		playAYearOfPlenty(playList, curPlayer);
	}
	else if (choice == "monopoly")
	{
		//playAMonopoly(playList, numPlayers, curPlayer);
	}
	else if (choice == "cancel")
	{
		return ;
	}
	else
	{
		playDevCard(playList, board, numPlayers, curPlayer);
	}

}

void GameLogic::playAKnight(Player playerList[], GameBoard *board, int numPlayers, int curPlayer)
{
	if (playerList[curPlayer].canPlayKnight())
	{
		MyEnum::print(MyEnum::colorToString(playerList[curPlayer].getColor()) + " has played a Knight Card");
		placeThief(playerList, board, numPlayers, curPlayer);
		playerList[curPlayer].playedAKnight();
	}
	else
	{
		MyEnum::print("No Knight Card to play");
	}

}

void GameLogic::buildARoad_DevCard(MyEnum::Color color, GameBoard *board)
{
	std::vector<int> possibleEdges;
	int buildThis = -1;

	possibleEdges = board->possibleRoadPositions(color);
	if (possibleEdges.size() == 0)
	{
		MyEnum::print("No valid positions for a road");
	}
	else
	{
		MyEnum::print(possibleEdges);
		while (buildThis < 0 || buildThis >= possibleEdges.size())
		{
			MyEnum::print("Build Here");
			cin >> buildThis;
		}

		if (!board->buildARoad(possibleEdges[buildThis], color))
		{
			MyEnum::print("Something wrong with buildARoad_DevCard at edge: " + std::to_string(possibleEdges[buildThis]) + " and color: " + MyEnum::colorToString(color));
		}
	}
}

void GameLogic::playARoadBuilding(Player playerList[], GameBoard *board, int curPlayer)
{
	MyEnum::Color color = playerList[curPlayer].getColor();

	if (playerList[curPlayer].canPlayRoadBuilder())
	{
		MyEnum::print(MyEnum::colorToString(color) + " has played a Road Building Card");
		buildARoad_DevCard(color, board);
		buildARoad_DevCard(color, board);

		playerList[curPlayer].playedARoadBuilder();
	}
	else
	{
		MyEnum::print("No road builder to play");
	}
}

void GameLogic::playAYearOfPlenty(Player playerList[], int curPlayer)
{
	std::vector<MyEnum::Resource> resourceCards = std::vector<MyEnum::Resource>();
	std::string resource = "";

	if (playerList[curPlayer].canPlayYearPlenty())
	{
		MyEnum::print(MyEnum::colorToString(playerList[curPlayer].getColor()) + " has played a Year of Plenty");
		MyEnum::print("Which 2 resources do you want:");
		while (resourceCards.size() < 2)
		{
			std::getline(std::cin, resource);
			
			if (resource == "wood")
				resourceCards.push_back(MyEnum::wood);
			else if (resource == "brick")
				resourceCards.push_back(MyEnum::brick);
			else if (resource == "wheat")
				resourceCards.push_back(MyEnum::wheat);
			else if (resource == "wool")
				resourceCards.push_back(MyEnum::wool);
			else if (resource == "ore")
				resourceCards.push_back(MyEnum::ore);
			
		}

		playerList[curPlayer].playedAYearPlenty(resourceCards);

	}
	else
	{
		MyEnum::print("No Year of Plenty to play");
	}
}



void GameLogic::playAMonopoly(Player playerList[], int numPlayers, int curPlayer, MyEnum::Resource type)
{
	//std::string resource = "";
	//MyEnum::Resource type;
	int amount = 0;
	std::vector<MyEnum::Resource> everybodiesResources = std::vector<MyEnum::Resource>();


	if (playerList[curPlayer].canPlayMonopoly())
	{
/*		MyEnum::print(MyEnum::colorToString(playerList[curPlayer].getColor()) + " has played a monopoly");
		MyEnum::print("Which resource do you want from everybody");
		while (resource != "wood" && resource != "brick" && resource != "wheat" && resource != "wool" && resource != "ore")
		{
			std::getline(std::cin, resource);
			
			if (resource == "wood")
				type = MyEnum::wood;
			else if (resource == "brick")
				type = MyEnum::brick;
			else if (resource == "wheat")
				type = MyEnum::wheat;
			else if (resource == "wool")
				type = MyEnum::wool;
			else if (resource == "ore")
				type = MyEnum::ore;
		}
*/
	//	MyEnum::print("Asking for " + MyEnum::resourceToString(type));

		for (int i = 0; i < numPlayers; i++)
		{
			if (i != curPlayer)
			{
				amount += playerList[i].monopoly(type);
			}
		}

		for (int i = 0; i < amount; i++)
		{
			everybodiesResources.push_back(type);
		}

		playerList[curPlayer].playedAMonopoly(everybodiesResources);
	}
	else
	{
		MyEnum::print("No Monopoly to play");
	}
}

void GameLogic::initDevCards(std::vector<MyEnum::DevCard> &deck)
{
	for (int i = 0; i < 25; i++)
	{
		deck[i] = MyEnum::knight;
		/*
		if (i < 14)
		{
			deck[i] = MyEnum::knight;
		}
		else if (i == 14 || i == 15)
		{
			deck[i] = MyEnum::roadBuilding;
		}
		else if (i == 16 || i == 17)
		{
			deck[i] = MyEnum::yearPlenty;
		}
		else if (i == 18 || i == 19)
		{
			deck[i] = MyEnum::monopoly;
		}
		else if (i < 25)
		{
			deck[i] = MyEnum::victoryPoint;
		}
		*/
		
	}
}

void GameLogic::checkLargestArmy(Player playerList[], int numPlayers, Player **largestArmy)
{
	int max = -1;
	int tempPlayer = 0;

	for (int i = 0; i < numPlayers; i++)
	{
		if (playerList[i].getKnightCount() > max)
		{
			max = playerList[i].getKnightCount();
			tempPlayer = i;
		}
	}

	if (!*largestArmy && max >= 3)
	{
		*largestArmy = &playerList[tempPlayer];
	}
	else if (*largestArmy && max > (*largestArmy)->getKnightCount())
	{
		*largestArmy = &playerList[tempPlayer];
	}
}

int GameLogic::checkLongestRoad(Player playerList[], GameBoard *board, int numPlayers, Player **longestRoad, int longestRoadCount)
{
	return board->findLongestRoad(playerList, numPlayers, longestRoad, longestRoadCount);
}

void GameLogic::pointsForAllPlayers(Player playerList[], GameBoard *board, int numPlayers, Player *largestArmy, Player *longestRoad)
{
	int points;
	MyEnum::Color color;

	for (int i = 0; i < numPlayers; i++)
	{
		color = playerList[i].getColor();
		points = pointsForPlayer(board, color, largestArmy, longestRoad);

		MyEnum::print(MyEnum::colorToString(color) + " has " + std::to_string(points) + " Victory Points");
	}
}

int GameLogic::pointsForPlayer(GameBoard *board, MyEnum::Color color, Player *largestArmy, Player *longestRoad)
{
	int points;

	points = board->getBuildingPoints(color);

	if (largestArmy && largestArmy->getColor() == color)
	{
		points += 2;
	}

	if (longestRoad && longestRoad->getColor() == color)
	{
		points += 2;
	}

	return points;
}

bool GameLogic::checkWinner(Player playerList[], GameBoard *board, int numPlayers, Player *largestArmy, Player *longestRoad)
{
	int points;
	MyEnum::Color color;

	for (int i = 0; i < numPlayers; i++)
	{
		color = playerList[i].getColor();
		points = pointsForPlayer(board, color, largestArmy, longestRoad);
		points += playerList[i].devCardPoints();

		if (points >= 10)
		{
			MyEnum::print(MyEnum::colorToString(color) + " has " + std::to_string(points) + " Victory Points!!");
			MyEnum::print(MyEnum::colorToString(color) + " has won the game!!");
			return true;
		}
	}

	return false;
}

/*
int main(int argc, char** argv) {
	
	int numPlayers = 0;
	int curIndex = 0;
	Player *curPlayer;
	Player *tempPlayer;
	Player *largestArmy = 0;
	Player *longestRoad = 0;
	Player playerList[4];
	std::vector<MyEnum::DevCard> developmentCards = std::vector<MyEnum::DevCard>(25);
	bool exit = false;
	char action;
	int longestRoadCount = 0;
	int temp = 0;

	srand (time(NULL));

	initDevCards(developmentCards);

	while (numPlayers > 4 || numPlayers < 2) {
		cout << "Number of Players (2 - 4)" << endl;
		cin >> numPlayers;
	}

	MyEnum::print("");

	GameBoard *board = new GameBoard();

	for (int i = 0; i < numPlayers; i++)
	{
		playerList[i] = Player(static_cast<MyEnum::Color>(i));
		MyEnum::print(playerList[i].print() + "\n");
	}

	//initial setup - double back order 0,1,2,3,3,2,1,0
	for (int i = 0; i < numPlayers * 2; i++)
	{
		if (i < numPlayers)
		{
			placeSettlementAndRoad(playerList[i], board);
		}
		else
		{
			int index = numPlayers - (i % numPlayers) - 1;
			placeSettlementAndRoad(playerList[index], board);
		}
	}

	std::vector<MyEnum::Resource> firstRoundResources;

	//get resources from your first settlements
	for (int i = 0; i < numPlayers; i++)
	{
		tempPlayer = &playerList[i];
		firstRoundResources = board->distributeFirstRoundResources(tempPlayer->getColor());
		tempPlayer->gainResourceCards(firstRoundResources);
	}


	//standard game loop
	do
	{
		curPlayer = &playerList[curIndex];
		MyEnum::print(curPlayer->getColorString()+ "'s Turn\n");
		MyEnum::print(curPlayer->getHand().print());
		MyEnum::print(curPlayer->getHand().printDevelopmentCards());

		MyEnum::print("Action");
		std::cin >> action;
		switch (action)
		{
			case 'q':
				exit = true;
				break;1
			case 'h':
				printHex(board);
				break;
			case 'p':
				board->print();
				break;
			case 'b':
				build(curPlayer, board, developmentCards);
				break;
			case 'r':
				roll(playerList, board, numPlayers, curIndex);
				break;
			case 'e':
				curIndex = (curIndex + 1) % numPlayers;
				break;
			case 't':
				trade(playerList, numPlayers, curIndex, board);
				break;
			case 'd':
				playDevCard(playerList, board, numPlayers, curIndex);
				break;
			case 'v':
				pointsForAllPlayers(playerList, board, numPlayers, largestArmy, longestRoad);
				break;
		}

		checkLargestArmy(playerList, numPlayers, &largestArmy);
		temp = checkLongestRoad(playerList, board, numPlayers, &longestRoad, longestRoadCount);

		if (temp > longestRoadCount)
		{
			longestRoadCount = temp;
		}

		if (largestArmy)
		{
			MyEnum::print(MyEnum::colorToString(largestArmy->getColor()) + " has the Largest Army, with " + std::to_string(largestArmy->getKnightCount()));
		}

		if (longestRoad)
		{
			MyEnum::print(MyEnum::colorToString(longestRoad->getColor()) + " has the Longest Road, with " + std::to_string(longestRoadCount));
		}

		if (checkWinner(playerList, board, numPlayers, largestArmy, longestRoad))
		{
			exit = true;
		}

		MyEnum::print("");
	}
	while (!exit);

	std::cout << "Good Bye" << std::endl;
	return 0;
}
*/









