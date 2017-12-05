

#include "Hand.h"

Hand::Hand()
{
	resourceCards = std::vector<int>(5);
	developmentCards = std::vector<int>(5);
}

void Hand::addResourceCards(std::vector<MyEnum::Resource> newCards)
{
	for (int i = 0; i < newCards.size(); i++)
	{
		switch(newCards[i])
		{
			case MyEnum::wood:
				resourceCards[MyEnum::wood] += 1;
				break;
			case MyEnum::wheat:
				resourceCards[MyEnum::wheat] += 1;
				break;
			case MyEnum::brick:
				resourceCards[MyEnum::brick] += 1;
				break;
			case MyEnum::wool:
				resourceCards[MyEnum::wool] += 1;
				break;
			case MyEnum::ore:
				resourceCards[MyEnum::ore] += 1;
				break;
			case MyEnum::water:
				MyEnum::print("Tried to add water to player hand");
				break;
			case MyEnum::desert:
				MyEnum::print("Tried to add desert to player hand");
				break;
		}
	}
}

void Hand::addDevelopmentCard(MyEnum::DevCard newCard)
{
	switch(newCard)
	{
		case MyEnum::knight:
			developmentCards[MyEnum::knight] += 1;
			break;
		case MyEnum::roadBuilding:
			developmentCards[MyEnum::roadBuilding] += 1;
			break;
		case MyEnum::yearPlenty:
			developmentCards[MyEnum::yearPlenty] += 1;
			break;
		case MyEnum::monopoly:
			developmentCards[MyEnum::monopoly] += 1;
			break;
		case MyEnum::victoryPoint:
			developmentCards[MyEnum::victoryPoint] += 1;
			break;
	}
}

bool Hand::canBuildSettlement()
{
	if (resourceCards[MyEnum::wood] >= 1 && resourceCards[MyEnum::wheat] >= 1
		&& resourceCards[MyEnum::brick] >= 1 && resourceCards[MyEnum::wool] >= 1)
	{
		return true;
	}

	return false;
}

bool Hand::canBuildRoad()
{
	if (resourceCards[MyEnum::wood] >= 1 && resourceCards[MyEnum::brick] >= 1)
	{
		return true;
	}

	return false;
}

bool Hand::canBuildCity()
{
	if (resourceCards[MyEnum::ore] >= 3 && resourceCards[MyEnum::wheat] >= 2)
	{
		return true;
	}

	return false;
}

bool Hand::canBuildDevCard()
{
	if (resourceCards[MyEnum::wheat] >= 1 && resourceCards[MyEnum::ore] >= 1
		&& resourceCards[MyEnum::wool] >= 1)
	{
		return true;
	}

	return false;
}

int Hand::handSize()
{
	int size = 0;

	for (int i = 0; i < 5; i++)
	{
		size += resourceCards[i];
	}

	return size;
}

int Hand::numberOfDevelopmentCards()
{
	int size = 0;

	for (int i = 0; i < 5; i++)
	{
		size += developmentCards[i];
	}

	return size;
}

bool Hand::removeResourceCards(std::vector<MyEnum::Resource> cards)
{

	for (int i = 0; i < cards.size(); i++)
	{
		switch(cards[i])
		{
			case MyEnum::wood: 
				resourceCards[MyEnum::wood] -= 1;
				break;
			case MyEnum::wheat:
				resourceCards[MyEnum::wheat] -= 1;
				break;
			case MyEnum::brick:
				resourceCards[MyEnum::brick] -= 1;
				break;
			case MyEnum::wool:
				resourceCards[MyEnum::wool] -= 1;
				break;
			case MyEnum::ore:
				resourceCards[MyEnum::ore] -= 1;
				break;
			case MyEnum::water:
				MyEnum::print("Tried to remove water from player hand");
				break;
			case MyEnum::desert:
				MyEnum::print("Tried to remove desert from player hand");
				break;

		}
	}

	if (!validResourceCards())
	{
		std::cout << "Have negative of a Resource Card, something broke" << std::endl;
		return false;
	}

	return true;
}

MyEnum::Resource Hand::removeResourceCardByIndex(int index)
{
	static const MyEnum::Resource arr[] = {MyEnum::wood, MyEnum::brick, MyEnum::wheat, MyEnum::wool, MyEnum::ore};
	std::vector<MyEnum::Resource> resources(arr, arr + sizeof(arr) / sizeof(arr[0])); 
	
	for (int i = 0; i < resources.size(); i++)
	{
		if (index < resourceCards[resources[i]])
		{
			resourceCards[resources[i]] -= 1;
			if (validResourceCards())
			{
				return resources[i];
			}
			else
			{
				MyEnum::print("Something wrong in removeResourceCardByIndex()");
			}
		}
		index -= resourceCards[resources[i]];
	}
	return MyEnum::water;
}

bool Hand::canGive(std::vector<MyEnum::Resource> cards)
{
	int wood = 0, brick = 0, wheat = 0, wool = 0, ore = 0;


	for (int i = 0; i < cards.size(); i++)
	{
		switch(cards[i])
		{
			case MyEnum::wood: 
				wood++;
				break;
			case MyEnum::wheat:
				wheat++;
				break;
			case MyEnum::brick:
				brick++;
				break;
			case MyEnum::wool:
				wool++;
				break;
			case MyEnum::ore:
				ore++;
				break;
			case MyEnum::water:
				MyEnum::print("Tried to remove water from player hand(canGive)");
				break;
			case MyEnum::desert:
				MyEnum::print("Tried to remove desert from player hand(canGive)");
				break;
		}
	}

	if (resourceCards[MyEnum::wood] < wood || resourceCards[MyEnum::brick] < brick || resourceCards[MyEnum::wheat] < wheat
		|| resourceCards[MyEnum::wool] < wool || resourceCards[MyEnum::ore] < ore)
	{
		return false;
	}

	return true;
}

bool Hand::validResourceCards()
{
	for (int i = 0; i < 5; i++)
	{
		if (resourceCards[i] < 0)
			return false;
	}

	return true;
}

bool Hand::validDevelopmentCards()
{
	for (int i = 0; i < 5; i++)
	{
		if (developmentCards[i] < 0)
			return false;
	}

	return true;
}

bool Hand::removeDevelopmentCards(std::vector<MyEnum::DevCard> cards)
{
	for (int i = 0; i < cards.size(); i++)
	{
		switch(cards[i])
		{
			case MyEnum::knight:
				developmentCards[MyEnum::knight] -= 1;
				break;
			case MyEnum::roadBuilding:
				developmentCards[MyEnum::roadBuilding] -= 1;
				break;
			case MyEnum::yearPlenty:
				developmentCards[MyEnum::yearPlenty] -= 1;
				break;
			case MyEnum::monopoly:
				developmentCards[MyEnum::monopoly] -= 1;
				break;
			case MyEnum::victoryPoint:
				developmentCards[MyEnum::victoryPoint] -= 1;
				break;
		}
	}

	if (!validDevelopmentCards())
	{
		std::cout << "Have negative of a Development Card, something broke" << std::endl;
		return false;
	}

	return true;
}

std::vector<int> Hand::getResourceCards()
{
	return resourceCards;
}

std::vector<int> Hand::getDevCards()
{
	return developmentCards;
}

bool Hand::hasKnight()
{
	if (developmentCards[MyEnum::knight] > 0)
	{
		return true;
	}

	return false;
}

bool Hand::removeKnight()
{
	developmentCards[MyEnum::knight] -= 1;

	return validDevelopmentCards();
}

bool Hand::removeRoadBuilder()
{
	developmentCards[MyEnum::roadBuilding] -= 1;

	return validDevelopmentCards();
}

bool Hand::removeYearPlenty()
{
	developmentCards[MyEnum::yearPlenty] -= 1;

	return validDevelopmentCards();
}

bool Hand::removeMonopoly()
{
	developmentCards[MyEnum::monopoly] -= 1;

	return validDevelopmentCards();
}

bool Hand::hasYearPlenty()
{
	if (developmentCards[MyEnum::yearPlenty] > 0)
	{
		return true;
	}

	return false;
}

bool Hand::hasRoadBuilding()
{
	if (developmentCards[MyEnum::roadBuilding] > 0)
	{
		return true;
	}

	return false;
}

bool Hand::hasMonopoly()
{
	if (developmentCards[MyEnum::monopoly] > 0)
	{
		return true;
	}

	return false;
}

int Hand::monopoly(MyEnum::Resource resource)
{
	int amount = resourceCards[resource];
	resourceCards[resource] = 0;

	if (!validResourceCards())
	{
		MyEnum::print("Something went wrong with the monopoly card, monopoly(hand)");
	}

	return amount;
}

int Hand::devCardPoints()
{
	return developmentCards[MyEnum::victoryPoint];
}

std::string Hand::printDevelopmentCards()
{
	std::string ret = "";

	if (numberOfDevelopmentCards() > 0)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < developmentCards[i]; j++)
			{
				switch(i)
				{
					case MyEnum::knight: 
						ret += "|Knight| ";
						break;
					case MyEnum::roadBuilding:
						ret += "|Road Building| ";
						break;
					case MyEnum::yearPlenty:
						ret += "|Year of Plenty| ";
						break;
					case MyEnum::monopoly:
						ret += "|Monopoly| ";
						break;
					case MyEnum::victoryPoint:
						ret += "|Victory Point| ";
						break;
				}
			}
		}
	}
	else
	{
		ret = "No Development Cards";
	}

	return ret;
}

std::string Hand::print()
{
	std::string ret = "";

	if (handSize() > 0)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < resourceCards[i]; j++)
			{
				switch(i)
				{
					case MyEnum::wood: 
						ret += "|Wood| ";
						break;
					case MyEnum::brick:
						ret += "|Brick| ";
						break;
					case MyEnum::wheat:
						ret += "|Wheat| ";
						break;
					case MyEnum::wool:
						ret += "|Wool| ";
						break;
					case MyEnum::ore:
						ret += "|Ore| ";
						break;
				}
			}
		}
	}
	else 
		ret = "Empty";

	return ret;
}





















