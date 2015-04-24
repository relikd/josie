#include "Level.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "PlayerControl.h"
#include "MainMenuScene.h"
#include <sstream>

USING_NS_CC;

int _res_index;
int _res_index_sub;

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

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//index to string for background loading
	std::ostringstream s;
		s << "backgrounds/bg_"<< _res_index <<"."<<_res_index_sub << ".png";


	//Add Background Image
	auto background = Sprite::create(s.str());
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);
	
	//Add TileMap
	auto map = TMXTiledMap::create("tilemaps/example_01.tmx");
	this->addChild(map, 0);
	for (const auto& child : map->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
	

	//Add and Start Backgroundmusic
	auto backgroundmusic = CocosDenshion::SimpleAudioEngine::getInstance();
	backgroundmusic->preloadBackgroundMusic("audio/MainMenuAmbienceTrack96bit.mp3");
	backgroundmusic->setBackgroundMusicVolume(1.0);
	backgroundmusic->playBackgroundMusic("audio/MainMenuAmbienceTrack96bit.mp3", true);

	//Add Player
	Player *josie = Player::initWithLevel(this);
	josie->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + 288));
	this->addChild(josie, 1);
	josie->scheduleUpdate();

	//Add Pause Button in upper right corner
	auto pause = MenuItemImage::create("buttons/pausebutton.png",
										"buttons/pausebutton.png",
										CC_CALLBACK_1(Level::pause,this));
	pause->setPosition(origin.x + visibleSize.width - pause->getContentSize().width,
						origin.y + visibleSize.height -pause->getContentSize().height);
	auto menu = Menu::create(pause, NULL);
			    menu->setPosition(Vec2::ZERO);
			    this->addChild(menu, 1);

    PlayerControl *pc = PlayerControl::initWithPlayer(josie);
    this->addChild(pc,-1);

	return true;

}


//Method Called by Pausebutton -> "goes back" to MainMenu
void Level::pause(Ref* pSender)
{
	//auto mainmenu = MainMenu::createScene();
	//Director::getInstance()->pushScene(mainmenu);
	Director::getInstance()->popScene();
}
