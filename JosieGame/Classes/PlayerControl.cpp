#include "PlayerControl.h"
#include "Level.h"
#include "Player.h"
#include "PlayerBoss.h"


using namespace cocos2d;

PlayerControl::PlayerControl() {
	this->_level = NULL;
	this->_listenerLevel = NULL;
}
PlayerControl::~PlayerControl() {
	if (!_level->isBossLevel())
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listenerLevel);
	CCLOG("PlayerControl destroyed");
}

PlayerControl* PlayerControl::initWithLevel(Level* level)
{
	PlayerControl *pc = new PlayerControl();
	pc->init();
	pc->autorelease();
	pc->_level = level;
	if (level->isBossLevel())
		pc->addBossControls();
	else
		pc->addLevelControls();

	pc->scheduleUpdate();
	return pc;
}

void PlayerControl::addLevelControls()
{
	_listenerLevel = EventListenerTouchOneByOne::create();
	_listenerLevel->setSwallowTouches(true);

	_listenerLevel->onTouchBegan = [&](Touch* touch, Event* event)
	{
		Vec2 p = touch->getLocation();
		Size screenSize = Director::getInstance()->getVisibleSize();

		if (p.x < screenSize.width/2) // left side screen
		{
			_level->player->stopRun();
			return true; // because we need to continue afterwards
		}
		else // right side screen
		{
			if (p.y < screenSize.height/2) // lower right side
			{
				_level->player->slide();
			}
			else // upper right side
			{
				_level->player->jump();
			}
		}
		return false;
	};

	_listenerLevel->onTouchEnded = [=](Touch* touch, Event* event)
	{
		_level->player->continueRun();
	};

	//listener->onTouchMoved = [=](Touch* touch, Event* event){};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_listenerLevel, 30);
}
MenuItemImage *_left;
MenuItemImage *_right;
MenuItemImage *_bjump;
MenuItemImage *_shoot;
void PlayerControl::addBossControls()
{
	float screenWidth = Director::getInstance()->getVisibleSize().width;

	_left = MenuItemImage::create("buttons/left.png","buttons/left.png");
	_right = MenuItemImage::create("buttons/right.png","buttons/right.png");
	_bjump = MenuItemImage::create("buttons/jump.png","buttons/jump.png", CC_CALLBACK_1(PlayerControl::test, this));
	_shoot = MenuItemImage::create("buttons/shoot.png","buttons/shoot.png");

	_left->setScale(0.7);
	_right->setScale(0.7);
	_bjump->setScale(0.7);
	_shoot->setScale(0.7);

	_left->setOpacity(128);
	_right->setOpacity(128);
	_bjump->setOpacity(128);
	_shoot->setOpacity(128);

	_left->setPosition(Vec2(150,300));
	_right->setPosition(Vec2(300,120));
	_bjump->setPosition(Vec2(screenWidth-300,120));
	_shoot->setPosition(Vec2(screenWidth-150,300));

	auto levelmenu = Menu::create(_left,_right,_bjump,_shoot, NULL);
	levelmenu->setPosition(Vec2::ZERO);
	_level->addChild(levelmenu,1);
}

void PlayerControl::update(float dt)
{
	if (_left->isSelected())
		_level->playerBoss->moveLeft();
	if (_right->isSelected())
		_level->playerBoss->moveRight();
	if (_shoot->isSelected())
		_level->playerBoss->shoot();
}


void PlayerControl::test(Ref* pSender) {
	CCLOG("test callback");
	//_level->player->jump();
}

