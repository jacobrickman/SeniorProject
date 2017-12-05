

#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <vector>
#include <string>
#include "MyEnum.h"

class Hand {
	
	std::vector<int> resourceCards;
	std::vector<int> developmentCards;

public:
	Hand();
	void addResourceCards(std::vector<MyEnum::Resource> newCards);
	void addDevelopmentCard(MyEnum::DevCard newCard);
	bool canBuildSettlement();
	bool canBuildRoad();
	bool canBuildCity();
	bool canBuildDevCard();
	int handSize();
	int numberOfDevelopmentCards();

	bool hasKnight();
	bool hasYearPlenty();
	bool hasRoadBuilding();
	bool hasMonopoly();
	int devCardPoints();

	bool removeKnight();
	bool removeRoadBuilder();
	bool removeYearPlenty();
	bool removeMonopoly();

	int monopoly(MyEnum::Resource resource);

	bool removeResourceCards(std::vector<MyEnum::Resource> cards);
	MyEnum::Resource removeResourceCardByIndex(int index);
	bool canGive(std::vector<MyEnum::Resource> cards);
	bool validResourceCards();
	bool removeDevelopmentCards(std::vector<MyEnum::DevCard> cards);
	bool validDevelopmentCards();
	std::vector<int> getResourceCards();
	std::vector<int> getDevCards();
	std::string printDevelopmentCards();
	std::string print();
};

#endif