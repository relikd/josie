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

Scene* LevelSelect::createScene() {
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
bool LevelSelect::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//Button for Level 1.1
	auto lvl1_1 = MenuItemImage::create(
			"buttons/levelselectbuttons/level_selectbutton1.1.png",
			"buttons/levelselectbuttons/level_selectbutton1.1.png",
			CC_CALLBACK_1(LevelSelect::startLevel, this, 1, 1));
	lvl1_1->setPosition(
			Vec2((origin.x + visibleSize.width / 10)  * 2,
					origin.y + visibleSize.height / 2));

	//Button for Level 1.2
	auto lvl1_2 = MenuItemImage::create(
			"buttons/levelselectbuttons/level_selectbutton1.2.png",
			"buttons/levelselectbuttons/level_selectbutton1.2.png",
			CC_CALLBACK_1(LevelSelect::startLevel, this, 1, 1));
	lvl1_2->setPosition(
			Vec2((origin.x + visibleSize.width / 10)  * 4,
					origin.y + visibleSize.height / 2));

	//Button for Level 1.3
	auto lvl1_3 = MenuItemImage::create(
			"buttons/levelselectbuttons/level_selectbutton1.3.png",
			"buttons/levelselectbuttons/level_selectbutton1.3.png",
			CC_CALLBACK_1(LevelSelect::startLevel, this, 0, 1));//TODO: ruft derzeit rnd level 0.1, soll 1.3 rufen, sobald0.1 eigenen utton hat
	lvl1_3->setPosition(
			Vec2((origin.x + visibleSize.width / 10)  * 6,
					origin.y + visibleSize.height / 2));

	//Button for BossLevel (1.0)
	auto lvl1_0 = MenuItemImage::create(
			"buttons/levelselectbuttons/level_selectbutton1.0.png",
			"buttons/levelselectbuttons/level_selectbutton1.0.png",
			CC_CALLBACK_1(LevelSelect::startLevel, this, 1, 0));
	lvl1_0->setPosition(
			Vec2((origin.x + visibleSize.width / 10) * 8,
					origin.y + visibleSize.height / 2));


	auto pause = MenuItemImage::create("buttons/pausebutton.png",
			"buttons/pausebutton.png",
			CC_CALLBACK_1(LevelSelect::backToMainMenu, this));
	pause->setPosition(
			Vec2(origin.x + visibleSize.width - pause->getContentSize().width,
					origin.y + visibleSize.height
							- pause->getContentSize().height));

	auto levelmenu = Menu::create(lvl1_1, lvl1_2,lvl1_3, lvl1_0, pause, NULL);
	levelmenu->setPosition(Vec2::ZERO);
	this->addChild(levelmenu, 1);

	return true;

}

void LevelSelect::backToMainMenu(Ref* pSender) {
	Director::getInstance()->popScene();
}

void LevelSelect::startLevel(Ref* pSender, int level, int sublevel) {
	auto levelxx = Level::initWithLevel(level, sublevel);
	Director::getInstance()->pushScene(levelxx);
}
