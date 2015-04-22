#include "PlayerControl.h"
#include "Player.h"

using namespace cocos2d;

PlayerControl::PlayerControl() {}
PlayerControl::~PlayerControl() {
	// TODO: controls wieder löschen wenn die Scene gewechselt wird!
	CCLOG("PlayerControl destroyed");
}

PlayerControl* PlayerControl::create()
{
	PlayerControl *pc = new PlayerControl();
	pc->init();
	pc->autorelease();
	pc->addLevelControls();
	return pc;
}

void PlayerControl::addLevelControls()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		Vec2 p = touch->getLocation();
		Size screenSize = Director::getInstance()->getVisibleSize();

		if (p.x < screenSize.width/2) // left side screen
		{
			Player::instance()->stopRun();
			return true; // because we need to continue afterwards
		}
		else // right side screen
		{
			if (p.y < screenSize.height/2) // lower right side
			{
				Player::instance()->slide();
			}
			else // upper right side
			{
				Player::instance()->jump();
			}
		}
		return false;
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		Player::instance()->continueRun();
	};

	//listener->onTouchMoved = [=](Touch* touch, Event* event){};

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void PlayerControl::addBossControls(){}

