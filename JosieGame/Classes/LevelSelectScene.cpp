#include "LevelSelectScene.h"
#include "Level.h"
#include "ShopScene.h"
#include "Cutscene.h"
#include "GameStateManager.h"

using namespace cocos2d;


/*
 * In this Scene the Menu has the Tag 206.
 * Attached to the Menu the Difficulty Select Button has the Tag 204.
 * In setRandomDifficulty() the Label (Tag 203) which indicates the difficulty
 * is always removed and added again because I found no way to "update" the
 * value/words of a Label
 *
 * DM
 */


LevelSelect::LevelSelect() : _level(0), _will_be_destroyed(false), _random_difficulty(0) {}

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
	
	Level *rnd = Level::initWithLevelAndDifficulty(0, 1,_random_difficulty);
	Director::getInstance()->replaceScene(rnd);
}


void LevelSelect::buildUI()
{
	LayerColor *background = LayerColor::create(Color4B::BLACK);

	MenuItemImage* lvl1_1 = buttonWithSublevel(1);
	MenuItemImage* lvl1_2 = buttonWithSublevel(2);
	MenuItemImage* lvl1_3 = buttonWithSublevel(3);
	MenuItemImage* lvl1_4 = buttonWithSublevel(0);

	// Random Level Button
	MenuItemImage* lvl_random = MenuItemImage::create(
			"buttons/levelselectbuttons/randomlevelbutton.png",
			"buttons/levelselectbuttons/randomlevelbutton.png",
			CC_CALLBACK_0(LevelSelect::startRandomLevel, this));
	lvl_random->setPosition(960, 200);
	
	// Random Level Difficulty Switch
	MenuItemImage* difficulty_switch = MenuItemImage::create(
			"buttons/levelselectbuttons/difficulty_unselected.png",
			"buttons/levelselectbuttons/difficulty_selected.png",
			CC_CALLBACK_0(LevelSelect::setRandomDifficulty,this));
	difficulty_switch->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	difficulty_switch->setPosition(560,12);
	difficulty_switch->setTag(204);

	Label* difficulty_label = Label::createWithTTF("Random Level \n Schwierigkeitsgrad:","fonts/Marker Felt.ttf",60,Size::ZERO,TextHAlignment::CENTER,TextVAlignment::TOP);
	difficulty_label->setPosition(Vec2(-(difficulty_switch->getContentSize().width/2) , difficulty_switch->getContentSize().height + 120));
	difficulty_switch->addChild(difficulty_label);

	// Random Level only if Boss Level won
	if (!GameStateManager::isRandomLevelUnlocked(_level)) {
		lvl_random->setEnabled(false);
		lvl_random->setOpacity(50);
		difficulty_switch->setOpacity(50);
		difficulty_switch->setEnabled(false);

	}

	// backToMenu Button
	MenuItemImage* backToMenu = MenuItemImage::create(
			"buttons/pausebutton.png",
			"buttons/pausebutton.png",
			CC_CALLBACK_0(Director::popScene, Director::getInstance()));
	backToMenu->setPosition(1920 - backToMenu->getContentSize().width, backToMenu->getContentSize().height);

	Menu* levelmenu = Menu::create(lvl1_1, lvl1_2, lvl1_3, lvl1_4, lvl_random, backToMenu,difficulty_switch, NULL);
	levelmenu->setPosition(Vec2::ZERO);
	levelmenu->setTag(206);
	this->addChild(background, -1);
	this->addChild(levelmenu, 1);

	this->setRandomDifficulty();
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


void LevelSelect::setRandomDifficulty(){

	_random_difficulty++;
	if(_random_difficulty == 4)
	{
		_random_difficulty = 1;
	}

	if(this->getChildByTag(206)->getChildByTag(204)->getChildByTag(203) != nullptr){
		this->getChildByTag(206)->getChildByTag(204)->removeChildByTag(203);
	}

	std::ostringstream diff_str;
	diff_str << _random_difficulty;
	Label* rnd_diff_show = Label::createWithTTF(diff_str.str(),"fonts/Marker Felt.ttf", 90);
	rnd_diff_show->setColor(Color3B::BLACK);
	rnd_diff_show->setPosition(Vec2(90.0,90.0));
	rnd_diff_show->setTag(203);

	this->getChildByTag(206)->getChildByTag(204)->addChild(rnd_diff_show);
}


