#include "Level.h"
#include "Player.h"
#include "TMXEdit.h"
#include "AudioUnit.h"
#include "MapController.h"
#include "PlayerControl.h"

using namespace cocos2d;

Level::Level() {
	audioUnit = nullptr;
	mapManager = nullptr;
}
Level::~Level() {
	CCLOG("~Level");
}

Level* Level::initWithLevel(int level, int sublevel) {
	Level *l = new Level();
	l->autorelease();

	l->audioUnit = AudioUnit::initWithBoss(false);
	l->audioUnit->playBackground();

	l->createUI(level, sublevel);
	l->addPauseButton();

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
	Player *player = Player::initWithLevel(this);
	player->setPlayerOnGround(216);
	mapManager->addChild(player);

	this->addChild(background);
	this->addChild(mapManager);
	this->addChild(PlayerControl::initWithPlayer(player));
}

void Level::addPauseButton() {
	MenuItemImage *pause = MenuItemImage::create("buttons/pausebutton.png",
			"buttons/pausebutton.png", CC_CALLBACK_0(Level::pauseGame, this));
	pause->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	pause->setPosition(1920, 1080);

	Menu *menu = Menu::create(pause, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}

void Level::pauseGame()
{
	audioUnit->stopBackground();
	delete audioUnit;
	Director::getInstance()->popScene();
}


//
// Level moving
//

void Level::moveLevelAtSpeed(float speed) {
	resetLevelPosition(-mapManager->getPositionX() + speed);
}

void Level::resetLevelPosition(float position) // 0.0f if no parameter
{
	mapManager->setPositionX(-position);
}

