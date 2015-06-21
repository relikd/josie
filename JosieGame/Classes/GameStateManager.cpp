#include "GameStateManager.h"

#include "cocos2d.h"
using namespace cocos2d;


GameStateManager::GameStateManager() {}
GameStateManager::~GameStateManager() {}

void GameStateManager::initDefaults()
{

	UserDefault *ud = UserDefault::getInstance();
	ud->setIntegerForKey("josie_credits", 0);
	ud->setIntegerForKey("josie_perk_damage", 1);
	ud->setIntegerForKey("josie_perk_shoot", 1);
	ud->setIntegerForKey("josie_perk_frequency", 1);
	ud->setIntegerForKey("josie_perk_playerspeed", 1);
	ud->setIntegerForKey("josie_perk_shied", 0);
	ud->setBoolForKey("josie_perk_extralife", false);
	ud->setIntegerForKey("music_volume", 100);
	ud->setIntegerForKey("sfx_volume", 100);
	ud->setStringForKey("josie_collected_coins", "");
	ud->setBoolForKey("josie_save_state", true);
	ud->flush(); // no flush to reset settings after restart

}

void GameStateManager::initSpriteCache()
{
	// preload animation frames
	SpriteFrameCache *sfc = SpriteFrameCache::getInstance();
	sfc->addSpriteFramesWithFile("josie/josiewalk.plist", "josie/josiewalk.png");
	sfc->addSpriteFramesWithFile("josie/josiestartmoving.plist", "josie/josiestartmoving.png");
	sfc->addSpriteFramesWithFile("josie/josiejump.plist", "josie/josiejump.png");
	sfc->addSpriteFramesWithFile("josie/josieexplosion.plist", "josie/josieexplosion.png");
	sfc->addSpriteFramesWithFile("tilemaps/coin.plist", "tilemaps/coin.png");
}

bool GameStateManager::isLevelUnlocked(int level, int sublevel)
{
	return true;
	if (level == 1 && sublevel == 1)
		return true; // first level
	if (sublevel==0)
		sublevel=4;
	if (getCoinsForLevel(level, sublevel-1) == -1)
		return false;
	return true;
}


//
// Coin Management
//

void GameStateManager::updateCoinsForLevel(int level, int sublevel, int coins)
{
	// save coins if more than last time
	int old_coins = GameStateManager::getCoinsForLevel(level, sublevel);
	if (old_coins < 0)
		old_coins = 0;
	if (old_coins < coins) {
		GameStateManager::addCoins(coins - old_coins);
		GameStateManager::setCoinsForLevel(level, sublevel, coins);
	}
}

void GameStateManager::addCoins(int coins)
{
	UserDefault *ud = UserDefault::getInstance();
	int credits = ud->getIntegerForKey("josie_credits", 0);
	credits += coins;
	if (credits < 0) credits = 0;
	ud->setIntegerForKey("josie_credits", credits);
	ud->flush();
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

int GameStateManager::getCoinsForLevel(int level, int sublevel)
{
	if (sublevel==0) sublevel=4;
	std::string str = UserDefault::getInstance()->getStringForKey("josie_collected_coins", "");
	int lvl_index = 4*(level-1) + (sublevel-1);
	if (lvl_index < str.length() && lvl_index>=0)
		return str[lvl_index]-1;
	return -1;
}

void GameStateManager::setCoinsForLevel(int level, int sublevel, int coins)
{
	if (sublevel==0) sublevel=4;
	UserDefault *ud = UserDefault::getInstance();
	std::string str = ud->getStringForKey("josie_collected_coins", "");
	int lvl_index = 4*(level-1) + (sublevel-1);
	if (lvl_index<0) return; // auto generated level -> -4
	if (lvl_index >= str.length()) {
		for (int i = 0; i <= lvl_index; i++) {
			if (str[i]==0)
				str[i]=1;
		}
		str[lvl_index+1] = 0;
	}
	str[lvl_index] = (unsigned char)coins+1;
	ud->setStringForKey("josie_collected_coins", str);
	ud->flush();
}
