#include "Player.h"

using namespace cocos2d;


Player::Player() {}

Player::~Player() {}

Player* Player::create()
{
    Player* pSprite = new Player();
    
    if (pSprite->initWithFile("josie/josie_static.png"))
    {
        pSprite->autorelease();
        pSprite->initOptions();
        pSprite->addEvents();
        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void Player::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
}

void Player::update(float dt)
{
	float screenWidth = Director::getInstance()->getVisibleSize().width;
	float newX = this->getPositionX()+7;
	if (newX > screenWidth) newX-=screenWidth;
	this->setPositionX(newX);
}

void Player::addEvents()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {   
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();

        if(rect.containsPoint(p))
        {
            return true; // to indicate that we have consumed it.
        }

        return false; // we did not consume this event, pass thru.
    };

    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        Player::touchEvent(touch, Vec2());
    };

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
}

void Player::touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _point)
{
	auto act = CCJumpBy::create(1.0,Vec2(0,0),200,1);
	this->runAction(act);
}
