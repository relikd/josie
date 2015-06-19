#include "MainMenuScene.h"
#include "Cutscene.h"
#include "LevelSelectScene.h"
#include "OptionScreen.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    MenuItemImage* closeItem = MenuItemImage::create(
                                           "buttons/endbutton_notpushed.png",
                                           "buttons/endbutton_pushed.png",
                                           CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - closeItem->getContentSize().height*3 - 20));

	MenuItemImage* startbutton = MenuItemImage::create(
											"buttons/startbutton.png",
											"buttons/startbutton_pushed.png",
											CC_CALLBACK_1(MainMenu::play,this));
	startbutton->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - startbutton->getContentSize().height*2 - 20));


	Menu* menu = Menu::create(closeItem,startbutton, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setTag(101);
	this->addChild(menu, 1);


    Sprite* josielogo = Sprite::create("josielogo.png");
    josielogo->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - josielogo->getContentSize().height));
    this->addChild(josielogo,1);
    
    Sprite* background = Sprite::create("backgrounds/bg_mountain72dpi.png");
    background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(background, 0);


    Sprite* testground = Sprite::create("testground.png");
    testground->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + testground->getContentSize().height/2));
    this->addChild(testground,1);

    //add OptionsScreenFunction
    this->addChild(OptionScreen::createOptionButton(1900,20),10);

    return true;
}


void MainMenu::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void MainMenu::play(Ref* pSender)
{
	LevelSelect *levelSelectScreen = LevelSelect::createSceneWithLevel(1);
	Director::getInstance()->pushScene(levelSelectScreen);
}

