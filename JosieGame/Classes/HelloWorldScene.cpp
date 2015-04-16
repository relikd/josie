#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "buttons/endbutton_notpushed.png",
                                           "buttons/endbutton_pushed.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - closeItem->getContentSize().height*3 - 20));



	auto startbutton = MenuItemImage::create(
											"buttons/startbutton.png",
											"buttons/startbutton_pushed.png",
											CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
	startbutton->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - startbutton->getContentSize().height*2 - 20));

	// create menu, it's an autorelease object
		    auto menu = Menu::create(closeItem,startbutton, NULL);
		    menu->setPosition(Vec2::ZERO);
		    this->addChild(menu, 1);

	// Adding the Josie-Logo to Layer 1
    auto josielogo = Sprite::create("josielogo.png");
    josielogo->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - josielogo->getContentSize().height));
    this->addChild(josielogo,1);
    
    
    // add "HelloWorld" splash screen"
    auto background = Sprite::create("backgrounds/bg_mountain72dpi.png");

    // position the sprite on the center of the screen
    background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(background, 0);

    auto backgroundmusic = CocosDenshion::SimpleAudioEngine::getInstance();
    backgroundmusic->preloadBackgroundMusic("audio/testmusic.mp3");
    backgroundmusic->setBackgroundMusicVolume(1.0);
    backgroundmusic->playBackgroundMusic("audio/testmusic.mp3",false);


    auto josie = Sprite::create("josie/josie_static.png");
    josie->setPosition(Vec2(origin.x + visibleSize.width/4, origin.y + josie->getContentSize().height));
    this->addChild(josie,1);
    auto testground = Sprite::create("testground.png");
    testground->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + testground->getContentSize().height/2));
    this->addChild(testground,1);
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
