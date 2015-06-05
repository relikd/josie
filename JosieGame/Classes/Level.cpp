#include "Level.h"
#include <sstream>
#include "Player.h"
#include "TMXEdit.h"
#include "AudioUnit.h"
#include "MapController.h"
#include "PlayerControl.h"

using namespace cocos2d;


Level::Level() {
	audioUnit = NULL;
	tileManager = NULL;
	moveable = NULL;
	currentLevel = -1;
	currentSubLevel = -1;
}
Level::~Level() {
	CCLOG("~Level");
}

Level* Level::initWithLevel(int level, int sublevel)
{
	Level *l = new Level();
	l->autorelease();

	l->currentLevel = level;
	l->currentSubLevel = sublevel;

	l->addBackground();
	l->addTilemap();
	l->addPlayer();
	l->addAudio();
	l->addPauseButton();

	return l;
}

//Method Called by Pausebutton -> "goes back" to MainMenu
void Level::pause(Ref* pSender) {
	audioUnit->stopBackground();
	delete audioUnit;
	delete tileManager;
	this->removeAllChildren();
	Director::getInstance()->popScene();
}


//
// Create the UI
//
void Level::addPauseButton() {
	MenuItemImage *pause = MenuItemImage::create("buttons/pausebutton.png", "buttons/pausebutton.png", CC_CALLBACK_1(Level::pause, this));
	pause->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	pause->setPosition(1920,1080);

	Menu *menu = Menu::create(pause, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}

void Level::addBackground() {
	//index to string for background loading
	std::ostringstream s;
	s << "backgrounds/bg_" << currentLevel << "." << currentSubLevel << ".png";

	//Add Background Image
	Sprite *background = Sprite::create(s.str());
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	this->addChild(background);
}

void Level::addTilemap() { //Add TileMap
	if (currentLevel == 0)
	{
		std::ostringstream mapstr;
		mapstr << "tilemaps/0.1.tmx";
		TMXTiledMap* map = TMXEdit::makeMap();

		tileManager = MapController::initWithObject(map);
	}
	else
	{
		tileManager = MapController::initWithLevel(this);
	}
	moveable = Layer::create() ;

	moveable->addChild(tileManager->map);

	this->addChild(moveable);
}

void Level::addPlayer()
{
	//Add Player
	Player *player = Player::initWithLevel(this);
	player->setPosition(Vec2(380, 512));
	this->addChild(player);

	PlayerControl *playerControl = PlayerControl::initWithPlayer(player);
	this->addChild(playerControl);
}

void Level::addAudio() {
	audioUnit = AudioUnit::initWithBoss(false);
	audioUnit->playBackground();
}


//
// Level moving
//

void Level::moveLevelAtSpeed(float speed){
	moveable->setPositionX( moveable->getPositionX() - speed );
	tileManager->mapOffsetX += speed;
}

void Level::resetLevelPosition(float position) // 0.0f if no parameter
{
	moveable->setPositionX( position );
	tileManager->mapOffsetX = position;
}

