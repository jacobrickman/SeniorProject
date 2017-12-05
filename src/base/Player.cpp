

#include "Player.h"

Player::Player()
{
	color = MyEnum::empty;
	hand = Hand();
	knightCount = 0;
}

Player::Player(MyEnum::Color color)
{
	this->color = color;
	hand = Hand();
	knightCount = 0;
}

std::vector<std::string> Player::canBuild()
{
	std::vector<std::string> ret = std::vector<std::string>();

	if (canBuildSettlement())
	{
		ret.push_back("settlement");
	}

	if (canBuildRoad())
	{
		ret.push_back("road");
	}

	if (canBuildCity())
	{
		ret.push_back("city");
	}

	if (canBuildDevelopmentCard())
	{
		ret.push_back("dev card");
	}

	return ret;
}

bool Player::canBuildSettlement()
{
	return hand.canBuildSettlement();
}

bool Player::buildSettlement()
{
	if (canBuildSettlement())
	{
		std::vector<MyEnum::Resource> settlement = std::vector<MyEnum::Resource>(4);
		settlement[0] = MyEnum::wood;
		settlement[1] = MyEnum::brick;
		settlement[2] = MyEnum::wheat;
		settlement[3] = MyEnum::wool;
		hand.removeResourceCards(settlement);

		return true;
	}

	return false;
}

bool Player::canBuildRoad()
{
	return hand.canBuildRoad();
}

bool Player::buildRoad()
{
	if (canBuildRoad())
	{
		std::vector<MyEnum::Resource> road = std::vector<MyEnum::Resource>(2);
		road[0] = MyEnum::wood;
		road[1] = MyEnum::brick;
		hand.removeResourceCards(road);

		return true;
	}

	return false;
}

bool Player::canBuildCity()
{
	return hand.canBuildCity();
}

bool Player::buildCity()
{
	if (canBuildCity())
	{
		std::vector<MyEnum::Resource> city = std::vector<MyEnum::Resource>(5);
		city[0] = MyEnum::ore;
		city[1] = MyEnum::ore;
		city[2] = MyEnum::ore;
		city[3] = MyEnum::wheat;
		city[4] = MyEnum::wheat;
		hand.removeResourceCards(city);

		return true;
	}

	return false;
}

bool Player::canBuildDevelopmentCard()
{
	return hand.canBuildDevCard();
}

bool Player::buildDevelopmentCard()
{
	if (canBuildDevelopmentCard())
	{
		std::vector<MyEnum::Resource> devCard = std::vector<MyEnum::Resource>(3);
		devCard[0] = MyEnum::wool;
		devCard[1] = MyEnum::wheat;
		devCard[2] = MyEnum::ore;
		hand.removeResourceCards(devCard);

		return true;
	}

	return false;
}

MyEnum::Resource Player::removeRandomCard()
{
	int card = rand() % hand.handSize();
	return hand.removeResourceCardByIndex(card);
}

void Player::rolledASeven()
{
	int numCards = hand.handSize();
	std::vector<MyEnum::Resource> resourceCards = std::vector<MyEnum::Resource>();
	std::string resource = "";

	if (numCards > 7)
	{
		MyEnum::print(MyEnum::colorToString(color) + " has " + std::to_string(numCards) + " cards, must get rid of " + std::to_string(numCards / 2) + " of them");
		MyEnum::print("Choose the " + std::to_string(numCards / 2) + " to get rid of");
		MyEnum::print(hand.print());

		while (resourceCards.size() < numCards / 2)
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

		if (canGiveResources(resourceCards))
		{
			removeResourceCards(resourceCards);
			MyEnum::print(hand.print());
		}
		else
		{
			rolledASeven();
		}
	}
	else
	{
		MyEnum::print(MyEnum::colorToString(color) + " has " + std::to_string(numCards) + " cards, doesn't do anything");
	}
}

bool Player::tryAgain()
{
	char ans = '\0';

	while (ans != 'y' && ans != 'n')
	{
		std::cin >> ans;
	}

	if (ans == 'y')
		return true;
	else
		return false;
}

bool Player::genPortTrade(int amount)
{
	int res = -1;
	int res2 = -1;
	std::vector<MyEnum::Resource> checkResources = std::vector<MyEnum::Resource>(amount);
	std::vector<MyEnum::Resource> giveResources = std::vector<MyEnum::Resource>(1);
	MyEnum::Resource resource;
	MyEnum::Resource resource2;

	MyEnum::print("Trade " + std::to_string(amount) + " of any one resource for one other resource");
	MyEnum::print("Which resource: Wood, Brick, Wheat, Wool, Ore, Cancel");

	while (res < 0 || res > 5)
	{
		std::cin >> res;
	}

	switch(res)
	{
		case 0:
			resource = MyEnum::wood;
			break;
		case 1:
			resource = MyEnum::brick;
			break;
		case 2:
			resource = MyEnum::wheat;
			break;
		case 3:
			resource = MyEnum::wool;
			break;
		case 4:
			resource = MyEnum::ore;
			break;
		case 5:
			return true;
	}

	MyEnum::print("For which resource: Wood, Brick, Wheat, Wool, Ore, Cancel");

	while (res2 < 0 || res2 > 5)
	{
		std::cin >> res2;
	}

	switch(res2)
	{
		case 0:
			resource2 = MyEnum::wood;
			break;
		case 1:
			resource2 = MyEnum::brick;
			break;
		case 2:
			resource2 = MyEnum::wheat;
			break;
		case 3:
			resource2 = MyEnum::wool;
			break;
		case 4:
			resource2 = MyEnum::ore;
			break;
		case 5:
			return true;
	}

	for (int i = 0; i < amount; i++)
	{
		checkResources[i] = resource;
	}

	if (canGiveResources(checkResources))
	{
		if (removeResourceCards(checkResources))
		{
			giveResources[0] = resource2;
			gainResourceCards(giveResources);
			return true;
		}
		else
		{
			MyEnum::print("Something went wrong, try again?");
			return !tryAgain();
		}	
	}
	else
	{
		MyEnum::print("Something went wrong, try agian?");
		return !tryAgain();
	}
}

bool Player::specialPortTrade(MyEnum::Port port)
{
	int res = -1;
	MyEnum::Resource resource;
	std::vector<MyEnum::Resource> tradeIn = std::vector<MyEnum::Resource>(2);
	std::vector<MyEnum::Resource> tradeOut = std::vector<MyEnum::Resource>(1);

	if (port == MyEnum::genPort || port == MyEnum::noPort)
	{
		MyEnum::print("specialPortTrade was called with a non special port");
		return true;
	}

	MyEnum::print("Which resource do you wish to trade 2 " + MyEnum::portToResourceString(port) + " for");
	MyEnum::print("Wood, Brick, Wheat, Wool, Ore, Cancel");
	while (res < 0 || res > 5)
	{
		std::cin >> res;
	}

	switch(res)
	{
		case 0:
			resource = MyEnum::wood;
			break;
		case 1:
			resource = MyEnum::brick;
			break;
		case 2:
			resource = MyEnum::wheat;
			break;
		case 3:
			resource = MyEnum::wool;
			break;
		case 4:
			resource = MyEnum::ore;
			break;
		case 5:
			return true;
	}

	tradeIn[0] = MyEnum::portToResource(port);
	tradeIn[1] = MyEnum::portToResource(port);

	tradeOut[0] = resource;

	if (canGiveResources(tradeIn))
	{
		if (removeResourceCards(tradeIn))
		{
			gainResourceCards(tradeOut);
			return true;
		}
		else
		{
			MyEnum::print("Something went wrong, try again?");
			return !tryAgain();
		}
	}
	else
	{
		MyEnum::print("Something went wrong, try again?");
		return !tryAgain();
	}
}

bool Player::portTrade(MyEnum::Port port)
{
	if (port == MyEnum::genPort)
	{
		return genPortTrade(3);
	}
	else if (port == MyEnum::noPort)
	{
		return genPortTrade(4);
	}
	else
	{
		return specialPortTrade(port);
	}
}

Hand Player::getHand()
{
	return hand;
}

void Player::gainResourceCards(std::vector<MyEnum::Resource> resources)
{
	hand.addResourceCards(resources);
}

void Player::gainDevelopmentCard(MyEnum::DevCard card)
{
	hand.addDevelopmentCard(card);
}

bool Player::removeResourceCards(std::vector<MyEnum::Resource> resources)
{
	if (canGiveResources(resources))
	{
		return hand.removeResourceCards(resources);
	}

	return false;
}

bool Player::removeDevelopmentCards(std::vector<MyEnum::DevCard> devcards)
{
	return hand.removeDevelopmentCards(devcards);
}

bool Player::canGiveResources(std::vector<MyEnum::Resource> resources)
{
	return hand.canGive(resources);
}

std::string Player::print()
{
	return MyEnum::colorToString(color) + "\n" + hand.print();
}

MyEnum::Color Player::getColor()
{
	return color;
}

bool Player::canPlayKnight()
{
	return hand.hasKnight();
}

bool Player::canPlayRoadBuilder()
{
	return hand.hasRoadBuilding();
}

bool Player::canPlayYearPlenty()
{
	return hand.hasYearPlenty();
}

bool Player::canPlayMonopoly()
{
	return hand.hasMonopoly();
}

std::string Player::getColorString()
{
	return MyEnum::colorToString(color);
}

void Player::playedAKnight()
{
	if (hand.removeKnight())
	{
		knightCount++;
	}
	else
	{
		MyEnum::print("Something went wrong in playedAKnight()");
	}
}

void Player::playedARoadBuilder()
{
	if (!hand.removeRoadBuilder())
	{
		MyEnum::print("Something went wrong in playedARoadBuilder()");
	}
}

void Player::playedAYearPlenty(std::vector<MyEnum::Resource> resources)
{
	if (hand.removeYearPlenty())
	{
		hand.addResourceCards(resources);
	}
	else
	{
		MyEnum::print("Something went wrong in playedAYearPlenty()");
	}
}

void Player::playedAMonopoly(std::vector<MyEnum::Resource> resources)
{
	if (hand.removeMonopoly())
	{
		hand.addResourceCards(resources);
	}
	else
	{
		MyEnum::print("Something went wrong in playedAMonopoly()");
	}
}

int Player::getKnightCount()
{
	return knightCount;
}

int Player::monopoly(MyEnum::Resource resource)
{
	return hand.monopoly(resource);
}

int Player::devCardPoints()
{
	return hand.devCardPoints();
}












