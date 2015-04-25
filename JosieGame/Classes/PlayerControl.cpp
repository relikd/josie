#include "PlayerControl.h"
#include "Player.h"
#include "Level.h"

using namespace cocos2d;

PlayerControl::PlayerControl() {
	this->_level = NULL;
	this->_listenerLevel = NULL;
}
PlayerControl::~PlayerControl() {
	Director::getInstance()->getEventDispatcher()->removeEventListener(_listenerLevel);
	CCLOG("PlayerControl destroyed");
}

PlayerControl* PlayerControl::initWithLevel(Level* level)
{
	PlayerControl *pc = new PlayerControl();
	pc->init();
	pc->autorelease();
	pc->addLevelControls();
	pc->_level = level;
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

void PlayerControl::addBossControls(){}

