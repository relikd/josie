#include "Level.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "PlayerControl.h"
#include <sstream>
#include "AudioUnit.h"

USING_NS_CC;

int _res_index;
int _res_index_sub;

Size visibleSize;
Vec2 origin;


Level::Level()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	player=NULL;
	playerControl=NULL;
	audioUnit=NULL;
}
Level::~Level(){}


Scene* Level::createScene(int level, int sublevel)
{

	_res_index = level;
	_res_index_sub = sublevel;

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Level::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Level::init()
{
	if (!Layer::init())
	{
		return false;
	}

	addBackground();
	
	addTilemap();

	addPauseButton();
	addAudio();
    addPlayer();
    addPlayerControl();
	return true;

}


//Method Called by Pausebutton -> "goes back" to MainMenu
void Level::pause(Ref* pSender)
{
	audioUnit->stopBackground();
	delete audioUnit;
	Director::getInstance()->popScene();
}

void Level::addTilemap()
{
	//Add TileMap
	//läd Tilemap aus Resource Ordner in Abhänggkeit der übergebenen level-Nummer
	std::ostringstream tilemap;
		tilemap << "tilemaps/"<< _res_index <<"."<<_res_index_sub << ".tmx";
	auto map = TMXTiledMap::create(tilemap.str());
	this->addChild(map, 0);
	for (const auto& child : map->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
}

void Level::addBackground()
{
	//index to string for background loading
	std::ostringstream s;
	s << "backgrounds/bg_"<< _res_index <<"."<<_res_index_sub << ".png";

	//Add Background Image
		auto background = Sprite::create(s.str());
		background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(background, 0);
}

void Level::addAudio()
{
	audioUnit = AudioUnit::initWithLevel(1,1);
	audioUnit->playBackground();
}

void Level::addPlayer()
{
	//Add Player
	player = Player::initWithLevel(this);
	player->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + 288));
	this->addChild(player, 1);
	player->scheduleUpdate();

}

void Level::addPlayerControl()
{
	//add control to scene
    playerControl = PlayerControl::initWithLevel(this);
    this->addChild(playerControl,-1);
}

void Level::addPauseButton()
{
	//Add Pause Button in upper right corner
	auto pause = MenuItemImage::create("buttons/pausebutton.png",
										"buttons/pausebutton.png",
										CC_CALLBACK_1(Level::pause,this));
	pause->setPosition(origin.x + visibleSize.width - pause->getContentSize().width,
						origin.y + visibleSize.height -pause->getContentSize().height);
	auto menu = Menu::create(pause, NULL);
			    menu->setPosition(Vec2::ZERO);
			    this->addChild(menu, 1);

}
