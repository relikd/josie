#include "Level.h"
#include "Player.h"
#include "TMXEdit.h"
#include "AudioUnit.h"
#include "MapController.h"
#include "PlayerControl.h"

using namespace cocos2d;

Level::Level() {
	audioUnit = nullptr;
	tileManager = nullptr;
	_currentLevel = -1;
	_currentSubLevel = -1;
}
Level::~Level() {
	CCLOG("~Level");
}

Level* Level::initWithLevel(int level, int sublevel) {
	Level *l = new Level();
	l->autorelease();

	l->_currentLevel = level;
	l->_currentSubLevel = sublevel;

	l->audioUnit = AudioUnit::initWithBoss(false);
	l->audioUnit->playBackground();

	l->createUI();
	l->addPauseButton();

	return l;
}


//
// Create the UI
//

void Level::createUI()
{
	// Background Image
	std::ostringstream bg_str;
	bg_str << "backgrounds/bg_" << _currentLevel << "." << _currentSubLevel << ".png";

	Sprite *background = Sprite::create(bg_str.str());
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	// Map Controller
	if (_currentLevel == 0)
		tileManager = MapController::initWithObject(TMXEdit::makeMap());
	else
		tileManager = MapController::initWithLevel(_currentLevel, _currentSubLevel);

	//Add Player
	Player *player = Player::initWithLevel(this);
	player->setPlayerOnGround(216);
	tileManager->map->addChild(player);

	this->addChild(background);
	this->addChild(tileManager->map);
	this->addChild(PlayerControl::initWithPlayer(player));
}

void Level::addPauseButton() {
	MenuItemImage *pause = MenuItemImage::create("buttons/pausebutton.png",
			"buttons/pausebutton.png", CC_CALLBACK_0(Level::pause, this));
	pause->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	pause->setPosition(1920, 1080);

	Menu *menu = Menu::create(pause, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}

void Level::pause()
{
	audioUnit->stopBackground();
	delete audioUnit;
	delete tileManager;
	this->removeAllChildren();
	Director::getInstance()->popScene();
}


//
// Level moving
//

void Level::moveLevelAtSpeed(float speed) {
	resetLevelPosition(-tileManager->map->getPositionX() + speed);
}

void Level::resetLevelPosition(float position) // 0.0f if no parameter
		{
	tileManager->map->setPositionX(-position);
	tileManager->mapOffsetX = position;
}

