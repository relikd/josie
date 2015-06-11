#include "LevelHUD.h"

using namespace cocos2d;

LevelHUD::LevelHUD() {
	txt_coins = nullptr;
	txt_time = nullptr;
	_timeRunning = 0;
	_previousSeconds = 0;
}
LevelHUD::~LevelHUD() {
	CCLOG("~LevelHUD");
}

LevelHUD* LevelHUD::initWithLevelName(const std::string& name)
{
	LevelHUD *hud = new LevelHUD();
	hud->autorelease();

	hud->addStatusBar(name);
	hud->addPauseButton();

	hud->scheduleUpdate();

	return hud;
}

void LevelHUD::addStatusBar(const std::string& title)
{
	LayerColor *blackbar = LayerColor::create(Color4B(0,0,0,255), 1920, 80);
	blackbar->setPosition(0, 1000); // layer anchor is 0,0

	Sprite *coinImage = Sprite::createWithSpriteFrameName("coin0000");
	coinImage->setScale( 65 / coin->getContentSize().height );
	coinImage->setPosition(62, 40);

	txt_coins = Label::createWithTTF("0 / 0", "fonts/Marker Felt.ttf", 48);
	txt_coins->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	txt_coins->setColor(Color3B(238, 156, 37));
	txt_coins->setPosition(125, 35);

	Label *levelname = Label::createWithTTF(title, "fonts/Marker Felt.ttf", 48);
	levelname->setPosition(1920/2, 35);

	txt_time = Label::createWithTTF("00:00", "fonts/Marker Felt.ttf", 48);
	txt_time->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	txt_time->setPosition(1900, 35);

	blackbar->addChild(coinImage);
	blackbar->addChild(txt_coins);
	blackbar->addChild(levelname);
	blackbar->addChild(txt_time);

	this->addChild(blackbar);
}

void LevelHUD::addPauseButton() {
	MenuItemImage *pause = MenuItemImage::create("buttons/pausebutton.png",
			"buttons/pausebutton.png", CC_CALLBACK_0(LevelHUD::pauseGame, this));
	pause->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	pause->setPosition(1900, 980);

	Menu *menu = Menu::create(pause, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}

void LevelHUD::pauseGame()
{
	Director::getInstance()->popScene();
}


//
// Gameplay Functionality
//

void LevelHUD::setCoins(int count, int max)
{
	char coins[10];
	sprintf(coins, "%d / %d", count, max);
	txt_coins->setString(coins);
}


//
// Other Functionality
//

void LevelHUD::update(float dt)
{
	_timeRunning += dt;
	updateStatusBar();
}

void LevelHUD::updateStatusBar()
{
	unsigned int minutes = (unsigned int)(_timeRunning / 60);
	unsigned int seconds = (unsigned int)(_timeRunning - (minutes*60));

	if (_previousSeconds != seconds) // update every second
	{
		_previousSeconds = seconds;
		char time[7];
		sprintf(time, "%02d:%02d", minutes, seconds);
		txt_time->setString(time);
	}
}
