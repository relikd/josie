#include "LevelGameOver.h"
#include "AudioUnit.h"

using namespace cocos2d;

LevelGameOver::LevelGameOver() {
	Director::getInstance()->pause();
	AudioUnit::stopBackground();
	AudioUnit::startBackgroundLevel();
	AudioUnit::pauseBackground();
}
LevelGameOver::~LevelGameOver() {
	AudioUnit::resumeBackground();
	Director::getInstance()->resume();
	CCLOG("~GameOverScreen");
}

LevelGameOver* LevelGameOver::createWin(int coins, int max_coins, float time)
{
	LevelGameOver *lgo = new LevelGameOver();
	if (lgo->init()) {
		lgo->autorelease();
		lgo->addSuccess(coins, max_coins, time);
	}
	return lgo;
}

LevelGameOver* LevelGameOver::createFail()
{
	LevelGameOver *lgo = new LevelGameOver();
	if (lgo->init()) {
		lgo->autorelease();
		lgo->addFail();
	}
	return lgo;
}

void LevelGameOver::addSuccess(int coins, int max_coins, float time)
{
	unsigned int minutes = (unsigned int) (time / 60);
	unsigned int seconds = (unsigned int) (time - (minutes * 60));
	unsigned int millis =  (unsigned int) ((time-((int)time))*1000);

	char time_str[7];
	char coin_str[10];
	sprintf(time_str, "%02d:%02d.%03d", minutes, seconds, millis);
	sprintf(coin_str, "%d / %d", coins, max_coins);


	LayerColor *bg = LayerColor::create(Color4B(4,103,51,255)); // dark green

	Label *lbl = Label::createWithTTF("Congratulation", "fonts/Marker Felt.ttf", 210);
	lbl->setColor(Color3B(255,237,0));
	lbl->setPosition(960, 840);

	Sprite *coinImage = Sprite::createWithSpriteFrameName("coin0001");
	Sprite *timeImage = Sprite::create("buttons/menubuttons/clock.png");
	coinImage->setPosition(715, 520);
	timeImage->setPosition(715, 320);

	Label *lbl_coins = Label::createWithTTF(coin_str, "fonts/Marker Felt.ttf", 120);
	Label *lbl_time = Label::createWithTTF(time_str, "fonts/Marker Felt.ttf", 120);
	lbl_coins->setColor(Color3B(238,156,37));
	lbl_coins->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	lbl_time->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	lbl_coins->setPosition(850, 505);
	lbl_time->setPosition(850, 305);

	MenuItemImage *retry = MenuItemImage::create("buttons/menubuttons/restart.png","buttons/menubuttons/restart.png", CC_CALLBACK_0(Director::popScene, Director::getInstance()));
	MenuItemImage *resume = MenuItemImage::create("buttons/menubuttons/resume.png","buttons/menubuttons/resume.png", CC_CALLBACK_0(Director::popToSceneStackLevel, Director::getInstance(), 2));
	retry->setPosition(200, 200);
	resume->setPosition(1720, 200);
	Menu *m = Menu::create(retry,resume,NULL);
	m->setPosition(Vec2::ZERO);

	this->addChild(bg);
	this->addChild(lbl);
	this->addChild(coinImage);
	this->addChild(lbl_coins);
	this->addChild(timeImage);
	this->addChild(lbl_time);
	this->addChild(m);
}

void LevelGameOver::addFail()
{
	LayerColor *bg = LayerColor::create(Color4B(235,11,22,255)); // diabolic red

	Label *lbl = Label::createWithTTF("Fail", "fonts/Marker Felt.ttf", 440);
	lbl->setColor(Color3B::BLACK);
	lbl->setPosition(960, 580);

	MenuItemImage *retry = MenuItemImage::create("buttons/menubuttons/restart.png", "buttons/menubuttons/restart.png", CC_CALLBACK_0(Director::popScene, Director::getInstance()));
	MenuItemImage *quit = MenuItemImage::create("buttons/menubuttons/backtomenu.png", "buttons/menubuttons/backtomenu.png", CC_CALLBACK_0(Director::popToSceneStackLevel, Director::getInstance(), 2));
	retry->setPosition(200, 200);
	quit->setPosition(1720, 200);
	Menu *m = Menu::create(retry, quit, NULL);
	m->setPosition(Vec2::ZERO);

	this->addChild(bg);
	this->addChild(lbl);
	this->addChild(m);
}
