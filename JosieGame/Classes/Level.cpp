#include "Level.h"
#include "TMXEdit.h"
#include "AudioUnit.h"
#include "MapController.h"
#include "LevelHUD.h"
#include "LevelPlayer.h"
#include "LevelGameOver.h"
#include "GameStateManager.h"

using namespace cocos2d;

Level::Level() {
	mapManager = nullptr;
	hud = nullptr;
	coins = 0;
	_level = 0;
	_sublevel = 0;
	AudioUnit::preloadLevelSounds();
}
Level::~Level() {
	CCLOG("~Level");
	AudioUnit::stopBackground();
	AudioUnit::unloadLevelSounds();
}

Level* Level::initWithLevel(int level, int sublevel) {
	Level *l = new Level();
	l->autorelease();

	l->_level = level;
	l->_sublevel = sublevel;
	AudioUnit::startBackgroundLevel();
	l->createUI(level, sublevel);
	l->startAfterDelay(2.0);

	return l;
}


//
// Create the UI
//

void Level::createUI(int lvl, int sublvl)
{
	// Background Image
	std::ostringstream bg_str;
	bg_str << "backgrounds/bg_" << lvl << "." << sublvl << ".png";
	Sprite *background = Sprite::create(bg_str.str());
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	// Map Controller
	if (lvl == 0)
		mapManager = TMXEdit::makeMap();
	else
		mapManager = MapController::initWithLevel(lvl, sublvl);

	//Add Player
	LevelPlayer *player = LevelPlayer::initWithLevel(this);
	player->setPlayerOnGround(400);

	char levelName[12];
	sprintf(levelName, "Level %d.%d", lvl, sublvl);
	hud = LevelHUD::initWithLevelName(levelName);
	hud->setCoins(0,4);

	this->addChild(background);
	this->addChild(mapManager);
	mapManager->addChild(player);
	this->addChild(hud);
}

void Level::startAfterDelay(float delay)
{
	hud->unscheduleUpdate(); // prevent player running

	Label *countdown = Label::createWithTTF("3", "fonts/Marker Felt.ttf", 80);
	countdown->setPosition(Director::getInstance()->getWinSize()/2);
	hud->addChild(countdown,-1);

	DelayTime *wait = DelayTime::create(delay/3.0);
	CallFuncN *set2 = CallFuncN::create(CC_CALLBACK_0(Label::setString, countdown, "2"));
	CallFuncN *set1 = CallFuncN::create(CC_CALLBACK_0(Label::setString, countdown, "1"));
	CallFuncN *rmLabel = CallFuncN::create(CC_CALLBACK_0(Node::removeFromParent, countdown));
	CallFuncN *startHUD = CallFuncN::create(CC_CALLBACK_0(Node::scheduleUpdate, hud));

	Sequence *seq = Sequence::create(wait,set2,wait,set1,wait,rmLabel,startHUD,nullptr);
	this->runAction(seq);
}


//
// Level Functions
//

void Level::moveLevelAtSpeed(float speed) {
	mapManager->setPositionX(mapManager->getPositionX() - speed);
}

void Level::resetLevelPosition(float position) // 0.0f if no parameter
{
	mapManager->setPositionX(-position);
	this->startAfterDelay(1.0);
}

void Level::addCoin()
{
	coins++;
	hud->setCoins(coins, 4);
}

void Level::finishLevelSuccessfull(bool successfull)
{
	resetLevelPosition();

	if (successfull) {
		// save coins if more than last time
		int old_coins = GameStateManager::getCoinsForLevel(_level,_sublevel);
		if (old_coins < coins) {
			GameStateManager::addCoins(coins-old_coins);
			GameStateManager::setCoinsForLevel(_level,_sublevel,coins);
		}

		LevelGameOver *gameover = LevelGameOver::createWin(coins, 4, hud->getTime());
		Director::getInstance()->pushScene(gameover);
	} else {
		LevelGameOver *gameover = LevelGameOver::createFail();
		Director::getInstance()->pushScene(gameover);
	}

	coins=-1;
	addCoin();
	hud->setTime(0);
	mapManager->reinitializeMap(false,true); // only coins
}
