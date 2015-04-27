#include "Level.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "PlayerBoss.h"
#include "PlayerControl.h"
#include <sstream>
#include "AudioUnit.h"
#include "MapController.h"

using namespace cocos2d;

Size visibleSize;
Vec2 origin;

Level::Level() {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	player = NULL;
	playerBoss = NULL;
	playerControl = NULL;
	audioUnit = NULL;
	tileManager = NULL;

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

	l->addBackground();
	l->addTilemap();
	l->addPauseButton();

	l->addAudio();
	l->addPlayer();
	l->addPlayerControl();

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
	this->addChild(tileManager->map,0);
}

void Level::addBackground() {
	//index to string for background loading
	std::ostringstream s;
	s << "backgrounds/bg_" << currentLevel << "." << currentSubLevel << ".png";

	//Add Background Image
	auto background = Sprite::create(s.str());
	background->setPosition(
			Vec2(visibleSize.width / 2 + origin.x,
					visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);
}

void Level::addAudio() {
	audioUnit = AudioUnit::initWithLevel(this);
	audioUnit->playBackground();
}

void Level::addPlayer() {
	//Add Player
	if (this->isBossLevel()) {
		playerBoss = PlayerBoss::initWithLevel(this);
		playerBoss->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y+188));
		this->addChild(playerBoss, 1);
	} else {
		player = Player::initWithLevel(this);
		player->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + 288));
		this->addChild(player, 1);
	}
}

void Level::addPlayerControl() {
	//add control to scene
	playerControl = PlayerControl::initWithLevel(this);
	this->addChild(playerControl, -1);
}

void Level::addPauseButton() {
	//Add Pause Button in upper right corner
	auto pause = MenuItemImage::create("buttons/pausebutton.png",
			"buttons/pausebutton.png", CC_CALLBACK_1(Level::pause, this));
	pause->setPosition(
			origin.x + visibleSize.width - pause->getContentSize().width,
			origin.y + visibleSize.height - pause->getContentSize().height);
	auto menu = Menu::create(pause, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

}
