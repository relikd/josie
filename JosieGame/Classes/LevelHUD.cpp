#include "LevelHUD.h"
#include "PauseScreen.h"

using namespace cocos2d;

LevelHUD::LevelHUD() {
	txt_coins = nullptr;
	txt_time = nullptr;
	_timeRunning = 0;
	_previousSeconds = 0;
	_stay = nullptr;
	_slide = nullptr;
	_jump = nullptr;
}
LevelHUD::~LevelHUD() {
	CCLOG("~LevelHUD");
}

LevelHUD* LevelHUD::initWithLevelName(const std::string& name)
{
	LevelHUD *hud = new LevelHUD();
	hud->autorelease();

	hud->addStatusBar(name);
	hud->addPlayerControls();
	hud->addChild(PauseScreen::createPauseButton(1900,980));

	hud->scheduleUpdate();

	return hud;
}


//
// Create UI
//

void LevelHUD::addStatusBar(const std::string& title)
{
	LayerColor *blackbar = LayerColor::create(Color4B(0,0,0,255), 1920, 80);
	blackbar->setPosition(0, 1000); // layer anchor is 0,0

	Sprite *coinImage = Sprite::createWithSpriteFrameName("coin0000");
	coinImage->setScale( 65 / coinImage->getContentSize().height );
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

void LevelHUD::addPlayerControls()
{
	_stay = MenuItemImage::create("buttons/left.png","buttons/left.png");
	_slide = MenuItemImage::create("buttons/right.png","buttons/right.png");
	_jump = MenuItemImage::create("buttons/jump.png","buttons/jump.png");

	_stay->setScale(0.8);
	_slide->setScale(0.8);
	_jump->setScale(0.8);

	_stay->setPosition(Vec2(150,150));
	_slide->setPosition(Vec2(1920-300,120));
	_jump->setPosition(Vec2(1920-150,300));

	Menu *menu = Menu::create(_stay,_slide,_jump, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setOpacity(128);
	this->addChild(menu);
}


//
// Update UI
//

void LevelHUD::setCoins(int count, int max)
{
	char coins[10];
	sprintf(coins, "%d / %d", count, max);
	txt_coins->setString(coins);
}

void LevelHUD::updateStatusBarTime(bool force)
{
	unsigned int minutes = (unsigned int)(_timeRunning / 60);
	unsigned int seconds = (unsigned int)(_timeRunning - (minutes*60));

	if (_previousSeconds != seconds || force == true) // update every second
	{
		_previousSeconds = seconds;
		char time[7];
		sprintf(time, "%02d:%02d", minutes, seconds);
		txt_time->setString(time);
	}
}


//
// Other Functionality
//

void LevelHUD::update(float dt)
{
	_timeRunning += dt;
	updateStatusBarTime();

	EventDispatcher *ed = Director::getInstance()->getEventDispatcher();

	if (_slide->isSelected())
		ed->dispatchCustomEvent("LEVEL_PLAYER_SLIDE_1");
	else
		ed->dispatchCustomEvent("LEVEL_PLAYER_SLIDE_0");

	if (_stay->isSelected())
		ed->dispatchCustomEvent("LEVEL_PLAYER_RUN_0");
	else
		ed->dispatchCustomEvent("LEVEL_PLAYER_RUN_1");

	if (_jump->isSelected())
		ed->dispatchCustomEvent("LEVEL_PLAYER_JUMP_1");
	else
		ed->dispatchCustomEvent("LEVEL_PLAYER_JUMP_0");
}
