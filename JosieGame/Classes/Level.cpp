#include "Level.h"
#include "Player.h"
#include "PlayerBoss.h"
#include <sstream>
#include "AudioUnit.h"
#include "MapController.h"
#include "HUD_Layer.h"

using namespace cocos2d;

Size visibleSize;
Vec2 origin;

Level::Level() {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	player = NULL;
	playerBoss = NULL;
	audioUnit = NULL;
	tileManager = NULL;
	HUD = NULL;
	moveable = NULL;

	currentLevel = -1;
	currentSubLevel = -1;
}
Level::~Level() {
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
	tileManager = MapController::initWithLevel(this);
	moveable = Layer::create() ;

	moveable->addChild(tileManager->map,0);

	this-> addChild(moveable,0);
}



void Level::addAudio() {
	audioUnit = AudioUnit::initWithLevel(this);
	audioUnit->playBackground();
}

void Level::addPlayer() {
	//Add Player
	if (this->isBossLevel()) {
		playerBoss = PlayerBoss::initWithLevel(this);
		playerBoss->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + 60));
		this->addChild(playerBoss, 1);
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
}

