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
	} else if (sublevel >= 1 && sublevel <= 3) {
		auto cut = Cutscene::createScene(sublevel);
		Director::getInstance()->pushScene(cut);
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


void LevelSelect::buildUI(){

	Sprite* background = Sprite::create("backgrounds/bg_lvl_select.png");
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background->setPosition(Vec2::ZERO);

	MenuItemImage* lvl1_1 = buttonWithSublevel(1);
	MenuItemImage* lvl1_2 = buttonWithSublevel(2);
	MenuItemImage* lvl1_3 = buttonWithSublevel(3);
	MenuItemImage* lvl1_4 = buttonWithSublevel(0);

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
}

MenuItemImage* LevelSelect::buttonWithSublevel(int sublevel)
{
	std::ostringstream btn_str;
	btn_str << "buttons/levelselectbuttons/level_selectbutton" << _level << "." << sublevel << ".png";

	MenuItemImage* itm = MenuItemImage::create(btn_str.str(), btn_str.str(), CC_CALLBACK_0(LevelSelect::startLevel, this, sublevel));
	itm->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	if (!GameStateManager::isLevelUnlocked(_level, sublevel)) {
		itm->setEnabled(false);
		itm->setOpacity(50);
	}

	if (sublevel==0) sublevel=4;
	itm->setPosition(10+(480*(sublevel-1)), 1070);

	return itm;
}

