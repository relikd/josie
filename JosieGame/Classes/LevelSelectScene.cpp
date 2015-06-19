#include "LevelSelectScene.h"
#include "Level.h"
#include "ShopScene.h"
#include "Cutscene.h"
#include "GameStateManager.h"

using namespace cocos2d;

LevelSelect::LevelSelect() : _level(0) {}

LevelSelect::~LevelSelect() {
	CCLOG("~SelectScreen");
}

LevelSelect* LevelSelect::createSceneWithLevel(int level)
{
	LevelSelect *ls = new LevelSelect();
	if (ls->init()) {
		ls->autorelease();
		ls->_level = level;
		ls->buildUI();
	}
	return ls;
}

void LevelSelect::startLevel(int sublevel) {
	if (sublevel == 0) {
		ShopScene *shop = ShopScene::initShop();
		Director::getInstance()->pushScene(shop);
	} else {
		Level *levelxx = Level::initWithLevel(_level, sublevel);
		Director::getInstance()->pushScene(levelxx);
	}
}

void LevelSelect::startRandomLevel()
{
	Level *rnd = Level::initWithLevel(0, 1);
	Director::getInstance()->pushScene(rnd);
}

void LevelSelect::testTalk(){
	auto cut = Cutscene::createScene();
	Director::getInstance()->pushScene(cut);
}

void LevelSelect::buildUI(){

	Sprite* background = Sprite::create("backgrounds/bg_lvl_select.png");
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background->setPosition(Vec2::ZERO);

	//Button for Level 1.1
	MenuItemImage* lvl1_1 = MenuItemImage::create(
			"buttons/levelselectbuttons/level_selectbutton1.1.png",
			"buttons/levelselectbuttons/level_selectbutton1.1.png",
			CC_CALLBACK_0(LevelSelect::testTalk, this));
	lvl1_1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	lvl1_1->setPosition(10, 1070);
	lvl1_1->setEnabled(false);
	lvl1_1->setOpacity(50);
	lvl1_1->setTag(301);

	//Button for Level 1.2
	MenuItemImage* lvl1_2 = MenuItemImage::create(
			"buttons/levelselectbuttons/level_selectbutton1.2.png",
			"buttons/levelselectbuttons/level_selectbutton1.2.png",
			CC_CALLBACK_0(LevelSelect::startLevel, this, 1));
	lvl1_2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	lvl1_2->setPosition(490, 1070);
	lvl1_2->setEnabled(false);
	lvl1_2->setOpacity(50);
	lvl1_2->setTag(302);

	//Button for Level 1.3
	MenuItemImage* lvl1_3 = MenuItemImage::create(
			"buttons/levelselectbuttons/level_selectbutton1.3.png",
			"buttons/levelselectbuttons/level_selectbutton1.3.png",
			CC_CALLBACK_0(LevelSelect::startLevel, this, 1)); //TODO: ruft derzeit rnd level 0.1, soll 1.3 rufen, sobald0.1 eigenen utton hat
	lvl1_3->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	lvl1_3->setPosition(970, 1070);
	lvl1_3->setEnabled(false);
	lvl1_3->setOpacity(50);
	lvl1_3->setTag(303);

	//Button for BossLevel (1.0)
	MenuItemImage* lvl1_4 = MenuItemImage::create(
			"buttons/levelselectbuttons/level_selectbutton1.0.png",
			"buttons/levelselectbuttons/level_selectbutton1.0.png",
			CC_CALLBACK_0(LevelSelect::startLevel, this, 0));
	lvl1_4->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	lvl1_4->setPosition(1450, 1070);
	lvl1_4->setEnabled(false);
	lvl1_4->setOpacity(50);
	lvl1_4->setTag(304);

	MenuItemImage* lvl_random = MenuItemImage::create(
			"buttons/levelselectbuttons/randomlevelbutton.png",
			"buttons/levelselectbuttons/randomlevelbutton.png",
			CC_CALLBACK_0(LevelSelect::startRandomLevel, this));
	lvl_random->setPosition(960, 210);

	MenuItemImage* pause = MenuItemImage::create(
			"buttons/pausebutton.png",
			"buttons/pausebutton.png",
			CC_CALLBACK_0(Director::popScene, Director::getInstance()));
	pause->setPosition(1920 - pause->getContentSize().width, pause->getContentSize().height);

	Menu* levelmenu = Menu::create(lvl1_1, lvl1_2, lvl1_3, lvl1_4, lvl_random, pause, NULL);
	levelmenu->setPosition(Vec2::ZERO);
	this->addChild(background, -1);
	this->addChild(levelmenu, 1);

	for (int i = 1; i <= cocos2d::UserDefault::getInstance()->getIntegerForKey("levels_complete"); i++) {
		((MenuItemImage*) levelmenu->getChildByTag(300 + i))->setEnabled(true);
		((MenuItemImage*) levelmenu->getChildByTag(300 + i))->setOpacity(255);
	}
}

