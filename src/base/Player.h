

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "MyEnum.h"
#include "Hand.h"

class Player {
	MyEnum::Color color;
	Hand hand;
	int knightCount;
	//int score;

public:
	Player();
	Player(MyEnum::Color color);

	std::vector<std::string> canBuild();

	bool canBuildSettlement();
	bool buildSettlement();
	bool canBuildRoad();
	bool buildRoad();
	bool canBuildCity();
	bool buildCity();
	bool canBuildDevelopmentCard();
	bool buildDevelopmentCard();

	void rolledASeven();

	bool canPlayKnight();
	bool canPlayRoadBuilder();
	bool canPlayYearPlenty();
	bool canPlayMonopoly();

	void playedAKnight();
	void playedARoadBuilder();
	void playedAYearPlenty(std::vector<MyEnum::Resource> resources);
	void playedAMonopoly(std::vector<MyEnum::Resource> resources);

	int monopoly(MyEnum::Resource resource);

	bool tryAgain();
	bool genPortTrade(int amount);
 	bool specialPortTrade(MyEnum::Port port);
	bool portTrade(MyEnum::Port port);

	int getKnightCount();
	Hand getHand();
	MyEnum::Color getColor();
	std::string getColorString();

	int devCardPoints();

	void gainResourceCards(std::vector<MyEnum::Resource> resources);
	void gainDevelopmentCard(MyEnum::DevCard cards);
	bool removeResourceCards(std::vector<MyEnum::Resource> resources);
	bool removeDevelopmentCards(std::vector<MyEnum::DevCard> devcards);
	MyEnum::Resource removeRandomCard();
	bool canGiveResources(std::vector<MyEnum::Resource> resources);
	std::string print();
};

#endif