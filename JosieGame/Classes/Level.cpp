#include "Level.h"
#include "TMXEdit.h"
#include "AudioUnit.h"
#include "MapController.h"
#include "LevelHUD.h"
#include "LevelPlayer.h"

using namespace cocos2d;

Level::Level() {
	mapManager = nullptr;
	hud = nullptr;
	coins = 0;
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

	AudioUnit::playLevelBackground();
	l->createUI(level, sublevel);

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
	player->setPlayerOnGround(216);

	char levelName[12];
	sprintf(levelName, "Level %d.%d", lvl, sublvl);
	hud = LevelHUD::initWithLevelName(levelName);
	hud->setCoins(0,4);

	this->addChild(background);
	this->addChild(mapManager);
	mapManager->addChild(player);
	this->addChild(hud);
}


//
// Level Functions
//

void Level::moveLevelAtSpeed(float speed) {
	resetLevelPosition(-mapManager->getPositionX() + speed);
}

void Level::resetLevelPosition(float position) // 0.0f if no parameter
{
	mapManager->setPositionX(-position);
}

void Level::addCoin()
{
	coins++;
	hud->setCoins(coins, 4);
}
