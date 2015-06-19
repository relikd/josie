/*
 * LevelSelectScene.cpp
 *
 *  Created on: 25.04.2015
 *      Author: danielmugge
 */

#include "LevelSelectScene.h"
#include "Level.h"
#include "ShopScene.h"
#include "Cutscene.h"
USING_NS_CC;

LevelSelect::LevelSelect() {}

LevelSelect::~LevelSelect() {
	CCLOG("~SelectScreen");
}

Scene* LevelSelect::createScene() {
	Scene* scene = Scene::create();

	LevelSelect* layer = LevelSelect::create();

	scene->addChild(layer);

	return scene;
}

bool LevelSelect::init() {
	if (!Layer::init()) {
		return false;
	}

	buildUI();

	return true;

}

void LevelSelect::backToMainMenu(Ref* pSender) {
	Director::getInstance()->popScene();
}

void LevelSelect::startLevel(Ref* pSender, int level, int sublevel) {
	if (sublevel == 0) {
		ShopScene *shop = ShopScene::initShop();
		Director::getInstance()->pushScene(shop);
	} else {
		Level *levelxx = Level::initWithLevel(level, sublevel);
		Director::getInstance()->pushScene(levelxx);
	}
}
void LevelSelect::testTalk(Ref* pSender){
	auto levelxxx = Cutscene::createScene();
	Director::getInstance()->pushScene(levelxxx);
}

void LevelSelect::buildUI(){
	Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		Sprite* background = Sprite::create("backgrounds/bg_lvl_select.png");
		background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		background->setPosition(Vec2::ZERO);

		//Button for Level 1.1
		MenuItemImage* lvl1_1 = MenuItemImage::create(
				"buttons/levelselectbuttons/level_selectbutton1.1.png",
				"buttons/levelselectbuttons/level_selectbutton1.1.png",
				CC_CALLBACK_1(LevelSelect::testTalk, this));
		lvl1_1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		lvl1_1->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - 10));
		lvl1_1->setEnabled(false);
		lvl1_1->setOpacity(50);
		lvl1_1->setTag(301);

		//Button for Level 1.2
		MenuItemImage* lvl1_2 = MenuItemImage::create(
				"buttons/levelselectbuttons/level_selectbutton1.2.png",
				"buttons/levelselectbuttons/level_selectbutton1.2.png",
				CC_CALLBACK_1(LevelSelect::startLevel, this, 1, 1));
		lvl1_2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		lvl1_2->setPosition(Vec2(origin.x + 490, origin.y + visibleSize.height - 10));
		lvl1_2->setEnabled(false);
		lvl1_2->setOpacity(50);
		lvl1_2->setTag(302);



		//Button for Level 1.3
		MenuItemImage* lvl1_3 = MenuItemImage::create(
				"buttons/levelselectbuttons/level_selectbutton1.3.png",
				"buttons/levelselectbuttons/level_selectbutton1.3.png",
				CC_CALLBACK_1(LevelSelect::startLevel, this, 1, 1));//TODO: ruft derzeit rnd level 0.1, soll 1.3 rufen, sobald0.1 eigenen utton hat
		lvl1_3->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		lvl1_3->setPosition(Vec2(origin.x + 970, origin.y + visibleSize.height - 10));
		lvl1_3->setEnabled(false);
		lvl1_3->setOpacity(50);
		lvl1_3->setTag(303);

		//Button for BossLevel (1.0)
		MenuItemImage* lvl1_4 = MenuItemImage::create(
				"buttons/levelselectbuttons/level_selectbutton1.0.png",
				"buttons/levelselectbuttons/level_selectbutton1.0.png",
				CC_CALLBACK_1(LevelSelect::startLevel, this, 1, 0));
		lvl1_4->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		lvl1_4->setPosition(Vec2(origin.x + 1450, origin.y + visibleSize.height - 10));
		lvl1_4->setEnabled(false);
		lvl1_4->setOpacity(50);
		lvl1_4->setTag(304);

		MenuItemImage* lvl_random = MenuItemImage::create(
				"buttons/levelselectbuttons/randomlevelbutton.png",
				"buttons/levelselectbuttons/randomlevelbutton.png",
				CC_CALLBACK_1(LevelSelect::startLevel, this, 0, 1));
		lvl_random->setPosition(Vec2(visibleSize.width/2, visibleSize.height/5 - 5));


		MenuItemImage* pause = MenuItemImage::create("buttons/pausebutton.png",
				"buttons/pausebutton.png",
				CC_CALLBACK_1(LevelSelect::backToMainMenu, this));
		pause->setPosition(
				Vec2(origin.x + visibleSize.width - pause->getContentSize().width,
						origin.y + pause->getContentSize().height));

		Menu* levelmenu = Menu::create(lvl1_1, lvl1_2,lvl1_3, lvl1_4,lvl_random, pause, NULL);
		levelmenu->setPosition(Vec2::ZERO);
		this->addChild(background,-1);
		this->addChild(levelmenu, 1);


		for(int i = 1; i <= cocos2d::UserDefault::getInstance()->getIntegerForKey("levels_complete"); i++){
			((MenuItemImage*)levelmenu->getChildByTag(300 + i))->setEnabled(true);
			((MenuItemImage*)levelmenu->getChildByTag(300 + i))->setOpacity(255);
		}
}

