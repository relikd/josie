#include "PlayerControl.h"

using namespace cocos2d;

PlayerControl::PlayerControl() {
	this->_player = NULL;
}
PlayerControl::~PlayerControl() {
	// TODO: controls wieder löschen wenn die Scene gewechselt wird!
	CCLOG("PlayerControl destroyed");
}

PlayerControl* PlayerControl::initWithPlayer(Player* player)
{
	PlayerControl *pc = new PlayerControl();
	pc->init();
	pc->autorelease();
	pc->addLevelControls();
	pc->_player = player;
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
			this->_player->stopRun();
			return true; // because we need to continue afterwards
		}
		else // right side screen
		{
			if (p.y < screenSize.height/2) // lower right side
			{
				this->_player->slide();
			}
			else // upper right side
			{
				this->_player->jump();
			}
		}
		return false;
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		this->_player->continueRun();
	};

	//listener->onTouchMoved = [=](Touch* touch, Event* event){};

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void PlayerControl::addBossControls(){}

