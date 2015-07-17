#include "GameStateManager.h"

#include "cocos2d.h"
using namespace cocos2d;

GameStateManager::GameStateManager() {}
GameStateManager::~GameStateManager() {}

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
		ud->setIntegerForKey("boss_difficulty", 1);
		ud->setBoolForKey("josie_no_cutscenes", false); // true after first boss win ?
		ud->setBoolForKey("josie_save_state", true);
		ud->flush();
	}
}

void GameStateManager::initSpriteCache() {
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
	if (level == 1 && sublevel == 1)
		return true; // first level
	
	switch (sublevel) {
		case 0: return (getCoinsForLevel(level, 3) != -1); // boss defeated?
		case 1: return (getCoinsForLevel(level-1, 0) != -1); // first sublevel of next level |or| random level
		default: return (getCoinsForLevel(level, sublevel - 1) != -1); // previous level finished?
	}
}

bool GameStateManager::isRandomLevelUnlocked(int level)
{
	return isLevelUnlocked(level+1, 1);
}

void GameStateManager::setBossDefeated(int level)
{
	setCoinsForLevel(level, 0, 0);
}

bool GameStateManager::showCutscenes() {
	return !UserDefault::getInstance()->getBoolForKey("josie_no_cutscenes", false);
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
	std::string str = UserDefault::getInstance()->getStringForKey("josie_collected_coins", "");
	int count = 0;
	for (int i = 0; i < str.length(); i++) {
		count += str[i] - 1;
	}
	return count;
}

void GameStateManager::updateCoinsForLevel(int level, int sublevel, int coins) {
	// save coins if more than last time
	int old_coins = getCoinsForLevel(level, sublevel);
	if (old_coins < 0)
		old_coins = 0;
	if (old_coins <= coins) {
		addCoins(coins - old_coins);
		setCoinsForLevel(level, sublevel, coins);
	}
}

void GameStateManager::updateTimeForLevel(int level, int sublevel, int time) {
	// save time if less than last time
	int old_time = getTimeForLevel(level, sublevel);
	if (time > 255)
		time = 255;
	if (old_time <= 0)
		old_time = 999;
	if (time <= old_time)
		setTimeForLevel(level, sublevel, time);
}

int GameStateManager::getCoinsForLevel(int level, int sublevel) {
	return getIntForLevel("josie_collected_coins", level, sublevel);
}
int GameStateManager::getTimeForLevel(int level, int sublevel) {
	return getIntForLevel("josie_time_taken", level, sublevel);
}


//
// private
//

void GameStateManager::setCoinsForLevel(int level, int sublevel, int coins) {
	setIntForLevel("josie_collected_coins", level, sublevel, coins);
}
void GameStateManager::setTimeForLevel(int level, int sublevel, int time) {
	setIntForLevel("josie_time_taken", level, sublevel, time);
}

int GameStateManager::getIntForLevel(const char* attribute, int level, int sublevel)
{
	if (sublevel == 0)
		level++;
	std::string str = UserDefault::getInstance()->getStringForKey(attribute, "");
	int lvl_index = 4 * (level - 1) + (sublevel - 1);
	if (lvl_index < str.length() && lvl_index >= 0)
		return str[lvl_index] - 1;
	return -1;
}

void GameStateManager::setIntForLevel(const char* attribute, int level, int sublevel, int new_val)
{
	if (sublevel == 0)
		level++;
	UserDefault *ud = UserDefault::getInstance();
	std::string str = ud->getStringForKey(attribute, "");
	int lvl_index = 4 * (level - 1) + (sublevel - 1);
	if (lvl_index < 0)
		return; // auto generated level -> -4
	if (lvl_index >= str.length()) {
		str.resize(lvl_index);
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
