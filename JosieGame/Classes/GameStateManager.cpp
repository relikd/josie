#include "GameStateManager.h"

#include "cocos2d.h"
using namespace cocos2d;

GameStateManager::GameStateManager() {
}
GameStateManager::~GameStateManager() {
}

void GameStateManager::initDefaults(bool force)
{
	UserDefault *ud = UserDefault::getInstance();
	if (force || ud->getBoolForKey("josie_save_state", false) == false) {
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
		ud->setStringForKey("josie_time_taken", "");
		ud->setBoolForKey("josie_save_state", true);
		ud->flush();
	}
}

void GameStateManager::initSpriteCache() {
	// preload animation frames
	SpriteFrameCache *sfc = SpriteFrameCache::getInstance();
	sfc->addSpriteFramesWithFile("josie/josiewalk.plist",
			"josie/josiewalk.png");
	sfc->addSpriteFramesWithFile("josie/josiestartmoving.plist",
			"josie/josiestartmoving.png");
	sfc->addSpriteFramesWithFile("josie/josiejump.plist",
			"josie/josiejump.png");
	sfc->addSpriteFramesWithFile("josie/josieexplosion.plist",
			"josie/josieexplosion.png");
	sfc->addSpriteFramesWithFile("tilemaps/coin.plist", "tilemaps/coin.png");
}

bool GameStateManager::isLevelUnlocked(int level, int sublevel) {
	return true;
	if (level == 1 && sublevel == 1)
		return true; // first level
	if (sublevel == 0)
		sublevel = 4;
	if (getCoinsForLevel(level, sublevel - 1) == -1)
		return false;
	return true;
}

//
// Coin Management
//

void GameStateManager::addCoins(int coins) {
	UserDefault *ud = UserDefault::getInstance();
	int credits = ud->getIntegerForKey("josie_credits", 0);
	credits += coins;
	if (credits < 0)
		credits = 0;
	ud->setIntegerForKey("josie_credits", credits);
	ud->flush();
}

int GameStateManager::getAllCollectedCoins() {
	std::string str = UserDefault::getInstance()->getStringForKey(
			"josie_collected_coins", "");
	int count = 0;
	for (int i = 0; i < str.length(); i++) {
		count += str[i] - 1;
	}
	return count;
}

void GameStateManager::updateCoinsForLevel(int level, int sublevel, int coins) {
	// save coins if more than last time
	int old_coins = GameStateManager::getCoinsForLevel(level, sublevel);
	if (old_coins < 0)
		old_coins = 0;
	if (old_coins < coins) {
		GameStateManager::addCoins(coins - old_coins);
		GameStateManager::setCoinsForLevel(level, sublevel, coins);
	}
}

void GameStateManager::updateTimeForLevel(int level, int sublevel, int time) {
	// save time if less than last time
	int old_time = GameStateManager::getTimeForLevel(level, sublevel);
	if (time > 255)
		time = 255;
	if (old_time < 0)
		old_time = 999;
	if (time < old_time)
		GameStateManager::setTimeForLevel(level, sublevel, time);
}

int GameStateManager::getCoinsForLevel(int level, int sublevel) {
	return GameStateManager::getIntForLevel("josie_collected_coins", level,
			sublevel);
}
int GameStateManager::getTimeForLevel(int level, int sublevel) {
	return GameStateManager::getIntForLevel("josie_time_taken", level, sublevel);
}
void GameStateManager::setCoinsForLevel(int level, int sublevel, int coins) {
	GameStateManager::setIntForLevel("josie_collected_coins", level, sublevel,
			coins);
}
void GameStateManager::setTimeForLevel(int level, int sublevel, int time) {
	GameStateManager::setIntForLevel("josie_time_taken", level, sublevel, time);
}

//
// private
//

static void getIntForLevel(const std::string& attribute, int level,
		int sublevel) {
	if (sublevel == 0)
		sublevel = 4;
	std::string str = UserDefault::getInstance()->getStringForKey(attribute,
			"");
	int lvl_index = 4 * (level - 1) + (sublevel - 1);
	if (lvl_index < str.length() && lvl_index >= 0)
		return str[lvl_index] - 1;
	return -1;
}

void GameStateManager::setIntForLevel(const std::string& attribute, int level,
		int sublevel, int new_val) {
	if (sublevel == 0)
		sublevel = 4;
	UserDefault *ud = UserDefault::getInstance();
	std::string str = ud->getStringForKey(attribute, "");
	int lvl_index = 4 * (level - 1) + (sublevel - 1);
	if (lvl_index < 0)
		return; // auto generated level -> -4
	if (lvl_index >= str.length()) {
		for (int i = 0; i <= lvl_index; i++) {
			if (str[i] == 0)
				str[i] = 1;
		}
		str[lvl_index + 1] = 0;
	}
	str[lvl_index] = (unsigned char) new_val + 1;
	ud->setStringForKey(attribute, str);
	ud->flush();
}
