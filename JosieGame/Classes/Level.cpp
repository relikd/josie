#include "Level.h"
#include "Player.h"
#include "PlayerBoss.h"
#include <sstream>
#include "AudioUnit.h"
#include "MapController.h"
#include "HUD_Layer.h"
#include "TMXEdit.h"
#include "Boss.h"

using namespace cocos2d;


Level::Level() {
	player = NULL;
	playerBoss = NULL;
	boss = NULL;
	audioUnit = NULL;
	tileManager = NULL;
	HUD = NULL;
	moveable = NULL;

	currentLevel = -1;
	currentSubLevel = -1;
}
Level::~Level() {CCLOG("del Level");
}

Level* Level::initWithLevel(int level, int sublevel)
{
	Level *l = new Level();
	l->autorelease();

	l->currentLevel = level;
	l->currentSubLevel = sublevel;

	l->addHUD();
	l->addTilemap();

	l->addAudio();
	l->addPlayer();

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

bool Level::isBossLevel()
{
	return (currentSubLevel == 0);
}



//
// ADD to Scene
//

void Level::addTilemap() { //Add TileMap
	if (currentLevel == 0){
		std::ostringstream mapstr;
			mapstr << "tilemaps/0.1.tmx";
			TMXTiledMap* map = TMXEdit::makeMap();

		tileManager = MapController::initWithObject(map);
	}
	else{
	tileManager = MapController::initWithLevel(this);
	}
	moveable = Layer::create() ;

	moveable->addChild(tileManager->map,0);

	this-> addChild(moveable,0);
}



void Level::addAudio() {
	audioUnit = AudioUnit::initWithLevel(this);
	audioUnit->playBackground();
}

void Level::addPlayer() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	//Add Player
	if (this->isBossLevel()) {
		playerBoss = PlayerBoss::initWithLevel(this);
		playerBoss->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + 60));
		this->addChild(playerBoss, 1);

		//Adding Boss in this method for testing
		boss = Boss::initWithLevel(this);
		boss->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height));
		this->addChild(boss,1);

	} else {
		player = Player::initWithLevel(this);
		player->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + 512));
		this->addChild(player, 1);
	}
}


void Level::addHUD(){
	auto toAdd = HUD_Layer::createForLevel(this);
	this-> addChild(toAdd, -1);
}

void Level::moveLevelAtSpeed(float speed){
	moveable->setPositionX( moveable->getPositionX() - speed );
	tileManager->mapOffsetX += speed;
}

void Level::resetLevelPosition(float position) // 0.0f if no parameter
{
	moveable->setPositionX( position );
	CCLOG("%1.3f", position);
	tileManager->mapOffsetX = position;
}

