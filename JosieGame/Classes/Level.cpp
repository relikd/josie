#include "Level.h"
#include "TMXEdit.h"
#include "AudioUnit.h"
#include "MapController.h"
#include "LevelHUD.h"
#include "LevelPlayer.h"
#include "LevelGameOver.h"
#include "LevelSelectScene.h"
#include "GameStateManager.h"

using namespace cocos2d;

Level::Level() {
	mapManager = nullptr;
	hud = nullptr;
	coins = 0;
	_level = 0;
	_sublevel = 0;
	_maxCoins = 0;
	_player = NULL;
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
	l->mapManager = MapController::initWithLevel(level, sublevel);
	
	l->createUI();

	return l;
}

Level* Level::initWithRandomLevel(int difficulty) {
	Level *l = new Level();
	l->autorelease();

	l->_level = 0;
	l->_sublevel = 1;
	l->mapManager = TMXEdit::makeMap(difficulty);
	
	l->createUI();

	return l;
}

void Level::scheduleHUD() { hud->scheduleUpdate(); }
void Level::unscheduleHUD() { hud->unscheduleUpdate(); }


//
// Create the UI
//

void Level::createUI()
{
	AudioUnit::startBackgroundLevel();
	
	//Add blueScreen
	LayerColor* blueScreen = LayerColor::create(Color4B(97,154,196,255));
	this->addChild(blueScreen);
	
	// Background Image
	std::ostringstream bg_str;
	bg_str << "backgrounds/bg_" << _level << "." << _sublevel << ".png";
	Sprite *background = Sprite::create(bg_str.str());
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	
	//Add Player
	_player = LevelPlayer::initWithLevel(this);
	_player->setPlayerOnGround(400);
	
	mapManager->setPlayerAsListener(_player);
	_maxCoins = mapManager->getMaxCoins();
	mapManager->addChild(_player);


	char levelName[12] = "Random";
	if (_level > 0) {
		sprintf(levelName, "Level %d.%d", _level, _sublevel);
	}
	hud = LevelHUD::initWithLevelName(levelName);
	hud->setCoins(0, _maxCoins);

	this->addChild(background);
	this->addChild(mapManager);
	this->addChild(hud);
	
	this->startAfterDelay(2.0);
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
	hud->setCoins(coins, _maxCoins);
}

void Level::finishLevelSuccessfull(bool successfull)
{
	resetLevelPosition();
	
	if (successfull) {
		GameStateManager::updateCoinsForLevel(_level, _sublevel, coins);
		GameStateManager::updateTimeForLevel(_level, _sublevel, (int)hud->getTime());
		LevelGameOver *gameover = LevelGameOver::createWin(coins, _maxCoins, hud->getTime());
		Director::getInstance()->pushScene(gameover);
	} else {
		LevelGameOver *gameover = LevelGameOver::createFail();
		Director::getInstance()->pushScene(gameover);
	}

	coins=-1;
	addCoin();
	hud->setTime(0);
	mapManager->reinitializeMap(false,true); // only coins
	mapManager->setPlayerAsListener(_player);
}
