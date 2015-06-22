#include "LevelSelectScene.h"
#include "Level.h"
#include "ShopScene.h"
#include "Cutscene.h"
#include "GameStateManager.h"

using namespace cocos2d;

LevelSelect::LevelSelect() : _level(0), _will_be_destroyed(false) {}

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
	if (_will_be_destroyed) return;
	_will_be_destroyed = true;
	
	if (sublevel == 0) {
		ShopScene *shop = ShopScene::initShop();
		Director::getInstance()->replaceScene(shop);
	} else {
		if (GameStateManager::showCutscenes()) {
			auto cut = Cutscene::createScene(sublevel);
			Director::getInstance()->replaceScene(cut);
		} else {
			Level *levelxx = Level::initWithLevel(_level, sublevel);
			Director::getInstance()->replaceScene(levelxx);
		}
	}
}

void LevelSelect::startRandomLevel()
{
	if (_will_be_destroyed) return;
	_will_be_destroyed = true;
	
	Level *rnd = Level::initWithLevel(0, 1);
	Director::getInstance()->replaceScene(rnd);
}


void LevelSelect::buildUI()
{
	LayerColor *background = LayerColor::create(Color4B::BLACK);

	MenuItemImage* lvl1_1 = buttonWithSublevel(1);
	MenuItemImage* lvl1_2 = buttonWithSublevel(2);
	MenuItemImage* lvl1_3 = buttonWithSublevel(3);
	MenuItemImage* lvl1_4 = buttonWithSublevel(0);

	MenuItemImage* lvl_random = MenuItemImage::create(
			"buttons/levelselectbuttons/randomlevelbutton.png",
			"buttons/levelselectbuttons/randomlevelbutton.png",
			CC_CALLBACK_0(LevelSelect::startRandomLevel, this));
	lvl_random->setPosition(960, 210);
	
	// Random Level only if Boss Level won
	if (!GameStateManager::isRandomLevelUnlocked(_level)) {
		lvl_random->setEnabled(false);
		lvl_random->setOpacity(50);
	}

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
	} else {
		if (sublevel!=0)
		{
			int coins = GameStateManager::getCoinsForLevel(_level, sublevel);
			int time = GameStateManager::getTimeForLevel(_level, sublevel);

			if (coins < 0)
				coins = 0;
			char txt_coin[4];
			sprintf(txt_coin, "%i", coins);

			char txt_time[7] = "---";
			if (time >= 0) {
				int minutes = (time / 60);
				int seconds = (time - (minutes * 60));
				sprintf(txt_time, "%02d:%02d", minutes, seconds);
			}

			Label *lbl_coin = Label::createWithTTF(txt_coin, "fonts/Marker Felt.ttf", 70);
			lbl_coin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			lbl_coin->setColor(Color3B(238, 156, 37));
			lbl_coin->setPosition(190, 362);
			itm->addChild(lbl_coin);

			Label *lbl_time = Label::createWithTTF(txt_time, "fonts/Marker Felt.ttf", 70);
			lbl_time->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			lbl_time->setColor(Color3B::BLACK);
			lbl_time->setPosition(190, 250);
			itm->addChild(lbl_time);
		}
	}

	if (sublevel==0) sublevel=4;
	itm->setPosition(10+(480*(sublevel-1)), Director::getInstance()->getWinSize().height-20);

	return itm;
}

