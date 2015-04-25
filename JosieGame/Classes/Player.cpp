#include <cmath>
#include "Player.h"
#include "Level.h"
#include "AudioUnit.h"

using namespace cocos2d;

const float _gravity = 9.81;
const float _jumpPower = 300;
const float _maxDeltaY = 20; // pixel
const float _runSpeed = 7.0;

float _upForce; // continuously changed during jump


Player::Player() {
	this->_level = NULL;
	_upForce=0;
	_isRunning=true;
	_isSliding=false;
	_isOnGround=false;
}
Player::~Player() {
	this->unscheduleUpdate();
	CCLOG("Player destroyed");
}

Player* Player::initWithLevel(Level* level)
{
	Player *pl = new Player();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("josie/JosieMoving.plist");
	pl->initWithSpriteFrameName("Josie-01.png");
	pl->autorelease();
	pl->setAnchorPoint(Vec2(0.5, 0));
	pl->_level = level;
	pl->runAction(pl->moving());
	pl->scheduleUpdate();

	return pl;
}

RepeatForever* Player::moving()
{
    // 3. repeat the frame
    int numFrame = 4;

    cocos2d::Vector<cocos2d::SpriteFrame *> frames;
    SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();

    char file[100] = {0};

	for (int i = 1; i < numFrame; i++) {
		sprintf(file, "Josie-%02d.png", i);
		SpriteFrame *frame = frameCache->getSpriteFrameByName(file);
		frames.pushBack(frame);
	}


    Animation *animation = Animation::createWithSpriteFrames(frames, 0.1);
    Animate *animate = Animate::create(animation);

    RepeatForever *repeat = RepeatForever::create(animate);
    return repeat;
}

void Player::update(float dt)
{
	this->_checkRun();
	this->_checkJump();
}


//
// Enter & Exit Scene
//

void Player::onEnterTransitionDidFinish()
{
   Node::onEnterTransitionDidFinish();
   CCLOG("player sprite open");
}

void Player::onExitTransitionDidStart()
{
   Node::onExitTransitionDidStart();
   CCLOG("player sprite close");
}


//
// Player interaction
//

void Player::run(bool r)
{
	if (_isRunning == r) return; // only update on state change

	_isRunning = r;

	if (!_isRunning) _level->audioUnit->playJosieStopRunSound();
}

void Player::jump()
{
	if (_isOnGround) {
		_upForce=_jumpPower;
		_level->audioUnit->playJosieJumpSound();
	}
}

void Player::slide(bool s)
{
	if (_isSliding == s) return; // don't update while sliding

	_isSliding = s;

	ScaleTo *scaleTo;
	SkewTo *skewTo;

	if (_isSliding) {
		scaleTo = ScaleTo::create(0.1, 1, 0.5); // scale y by half in 0.1sec
		skewTo = SkewTo::create(0.1, -30, 0);
		_level->audioUnit->playJosieSlideSound();
	} else {
		scaleTo = ScaleTo::create(0.1, 1, 1);
		skewTo = SkewTo::create(0.1, 0, 0);
	}

	this->runAction(scaleTo);
	this->runAction(skewTo);
}




//
// private functions
//

void Player::_checkRun()
{
	if (_isRunning){

		Vec2 myOrigin = this->getBoundingBox().origin;
		Size mySize = this->getContentSize();
		myOrigin.x += mySize.width + _runSpeed;
		TilePropertyType first = _level->getTileProperty(myOrigin);
		myOrigin.y += mySize.height;
		TilePropertyType second = _level->getTileProperty(myOrigin);

		if (first != TilePropertyCollision && second != TilePropertyCollision)
		{
			float newX = this->getPositionX();
			newX += _runSpeed;
			float screenWidth = Director::getInstance()->getVisibleSize().width;
			if (newX > screenWidth) newX-=screenWidth;
			this->setPositionX(newX);
		}
	}
}

void Player::_checkJump()
{
	Vec2 oldPos = this->getPosition();

	_upForce = fmax(_upForce - _gravity, 0);

	if (_upForce > 0.01) // as long as jump force is stronger than gravity
	{
		float deltaY = _maxDeltaY*(_upForce/_jumpPower);
		this->setPositionY(oldPos.y+deltaY);
		_isOnGround=false;
	}
	else
	{
		oldPos.y -= _gravity;
		TilePropertyType tpt = _level->getTileProperty(oldPos);
		if (tpt != TilePropertyCollision) {
			this->setPosition(oldPos);
			_isOnGround = false; // in case Josie falls of the cliff
		} else {
			_isOnGround = true;
		}
	}
}

