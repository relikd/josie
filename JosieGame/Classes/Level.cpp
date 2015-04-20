#include "Level.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "HelloWorldScene.h"


USING_NS_CC;

Scene* Level::createScene()
{
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

	//Add Background Image
	auto background = Sprite::create("backgrounds/bg_mountain72dpi.png");
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
	backgroundmusic->preloadBackgroundMusic("audio/testmusic.mp3");
	backgroundmusic->setBackgroundMusicVolume(1.0);
	backgroundmusic->playBackgroundMusic("audio/testmusic.mp3", false);

	//Add Player
	Player *josie = Player::create();
	josie->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + josie->getContentSize().height/2 + 288));
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



	return true;

}

// I Think this Method has no Use in LevelScene (Daniel Mügge)
void Level::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

//Method Called by Pausebutton -> "goes back" to MainMenu
void Level::pause(Ref* pSender)
{
	auto mainmenu = HelloWorld::createScene();
	Director::getInstance()->pushScene(mainmenu);
}
