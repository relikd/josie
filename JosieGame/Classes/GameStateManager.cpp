#include "GameStateManager.h"

#include "cocos2d.h"
using namespace cocos2d;


GameStateManager::GameStateManager() {}
GameStateManager::~GameStateManager() {}

void GameStateManager::initManagerWithDefaults()
{
	UserDefault *ud = UserDefault::getInstance();
}

bool GameStateManager::isLevelUnlocked(int level, int sublevel)
{
	if (getCoinsForLevel(level, sublevel-1) == -1)
		return false;
	return true;
}

int GameStateManager::getCoinsForLevel(int level, int sublevel)
{
	std::string str = UserDefault::getInstance()->getStringForKey("josie_collected_coins", "");
	int lvl_index = 3*(level-1) + (sublevel-1); // every level set has 3 sublevels
	if (lvl_index<0)
		return 0;
	if (lvl_index < str.length())
		return str[lvl_index]-1;
	return -1;
}

int GameStateManager::getAllCollectedCoins()
{
	std::string str = UserDefault::getInstance()->getStringForKey("josie_collected_coins", "");
	int count = 0;
	for (int i=0; i < str.length(); i++) {
		count += str[i]-1;
	}
	return count;
}

void GameStateManager::setCoinsForLevel(int level, int sublevel, int coins)
{
	UserDefault *ud = UserDefault::getInstance();
	std::string str = ud->getStringForKey("josie_collected_coins", "");
	int lvl_index = 3*(level-1) + (sublevel-1); // every level set has 3 sublevels
	if (lvl_index >= str.length()) {
		for (int i = 0; i <= lvl_index; i++) {
			if (str[i]==0)
				str[i]=1;
		}
		str[lvl_index+1] = 0;
	}
	str[lvl_index] = (unsigned char)coins+1;
	ud->setStringForKey("josie_collected_coins", str);
	if (SAVE_USER_STATE) ud->flush();
}

void GameStateManager::addCoins(int coins)
{
	UserDefault *ud = UserDefault::getInstance();
	int credits = ud->getIntegerForKey("josie_credits", 0);
	credits += coins;
	if (credits < 0) credits = 0;
	ud->setIntegerForKey("josie_credits", credits);
	if (SAVE_USER_STATE) ud->flush();
}
