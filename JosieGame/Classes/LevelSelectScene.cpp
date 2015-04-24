/*
 * LevelSelectScene.cpp
 *
 *  Created on: 25.04.2015
 *      Author: danielmugge
 */

#include "LevelSelectScene.h"
#include "Level.h"


USING_NS_CC;

LevelSelect::LevelSelect() {
	CCLOG("SlectScreen generated");

}

LevelSelect::~LevelSelect() {
	CCLOG("SelectScreen destroyed");
}


Scene* LevelSelect::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = LevelSelect::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LevelSelect::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto Level1_1 = LabelTTF::create("Level 1.1", "fonts/Marker Felt.ttf", 70);
    	auto lvl1_1 = MenuItemLabel::create(Level1_1,
    			CC_CALLBACK_1(LevelSelect::startLevel, this,1 ,1));
    	lvl1_1->setPosition(
    			Vec2(
    					origin.x + visibleSize.width / 2,
    					origin.y + visibleSize.height/ 2));

    auto pause = MenuItemImage::create("buttons/pausebutton.png",
    									"buttons/pausebutton.png", CC_CALLBACK_1(LevelSelect::backToMainMenu, this));
	pause->setPosition(Vec2(
			origin.x + visibleSize.width - pause->getContentSize().width,
			origin.y + visibleSize.height - pause->getContentSize().height));

    auto levelmenu = Menu::create(lvl1_1,pause, NULL);
    levelmenu->setPosition(Vec2::ZERO);
    this->addChild(levelmenu,1);

    return true;

}


void LevelSelect::backToMainMenu(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void LevelSelect::startLevel(Ref* pSender,int level,int sublevel)
{
	auto levelxx = Level::createScene(level, sublevel);
	Director::getInstance()->pushScene(levelxx);
}
