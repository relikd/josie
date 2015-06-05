#include <cmath>
#include "Player.h"
#include "Level.h"
#include "AudioUnit.h"
#include "MapController.h"

using namespace cocos2d;

#define DEFAULT_PLAYER_SCALE 0.5

const float _gravity = 9.81;
const float _jumpPower = 300;
const float _maxDeltaY = 20; // pixel
const float _runSpeed = 7.0;

float _upForce; // continuously changed during jump
float _timeDiff = 0.0;

Player::Player() {
	_level = nullptr;
	_animationSprite = nullptr;
	_upForce = 0;
	_isRunning = true;
	_isSliding = false;
	_isOnGround = false;
}
Player::~Player() {
	this->unscheduleUpdate();
	CCLOG("~Player");
}

Player* Player::initWithLevel(Level* level) {
	Player *pl = new Player();
	if (pl->initWithSize(160,245))
	{
		pl->autorelease();
		pl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		pl->setScale(DEFAULT_PLAYER_SCALE);
		pl->_level = level;

		pl->_animationSprite = Sprite::createWithSpriteFrameName("josiestartmove0000");
		pl->_animationSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		pl->_animationSprite->setPosition(pl->getContentSize().width/2-5, -5);
		pl->addChild(pl->_animationSprite);

		pl->_animationSprite->runAction(pl->animationWithFrame("josiewalk", 5));
		pl->scheduleUpdate();
	}

	return pl;
}

RepeatForever* Player::animationWithFrame(const std::string& name, int frameCount)
{
	Vector<SpriteFrame *> frames;
	SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();

	char file[50] = { 0 };

	for (int i = 0; i <= frameCount; i++) {
		sprintf(file, "%s%04d", name.c_str(), i);
		SpriteFrame *frame = frameCache->getSpriteFrameByName(file);
		frames.pushBack(frame);
	}

	Animation *animation = Animation::createWithSpriteFrames(frames, 0.1);
	Animate *animate = Animate::create(animation);

	RepeatForever *repeat = RepeatForever::create(animate);
	return repeat;
}


void Player::update(float dt) {
	this->_checkRun();
	this->_checkJump();
	_timeDiff += dt;
	if (_timeDiff > 0.33f) {
		_timeDiff = 0.0;
		this->_checkAlive();
	}
	_level->tileManager->tryCollect(this->getBoundingBox());
}

//
// Player interaction
//

void Player::run(bool r) {
	if (_isRunning == r)
		return; // only update on state change

	_isRunning = r;

	if (!_isRunning)
		_level->audioUnit->playJosieStopRunSound();
}

void Player::jump() {
	if (_isOnGround && !_isSliding) {
		_upForce = _jumpPower;
		_level->audioUnit->playJosieJumpSound();
	}
}

void Player::slide(bool s) {
	if (_isSliding == s)
		return; // don't update while sliding
	if (_isSliding && !this->_canStandUp())
		return; // keep sliding

	_isSliding = s;

	ScaleTo *scaleTo;
	SkewTo *skewTo;

	if (_isSliding) {
		scaleTo = ScaleTo::create(0.1, DEFAULT_PLAYER_SCALE, 0.2); // scale y to 0.3 in 0.1sec
		skewTo = SkewTo::create(0.1, -30, 0);
		_level->audioUnit->playJosieSlideSound();
	} else {
		scaleTo = ScaleTo::create(0.1, DEFAULT_PLAYER_SCALE, DEFAULT_PLAYER_SCALE);
		skewTo = SkewTo::create(0.1, 0, 0);
	}

	this->runAction(scaleTo);
	this->runAction(skewTo);
}

//
// private functions
//

bool Player::_canStandUp() {
	if (!_isSliding)
		return true; // already standing

	float air = _level->tileManager->collisionDiffTop(this->getBoundingBox());
	float scaleY = this->getScaleY();
	float height = this->getBoundingBox().size.height;

	// TODO: evtl. Skalierung in die Breite beachten
	return ((height / (scaleY / DEFAULT_PLAYER_SCALE)) - height -3.0) < air;
}

void Player::_checkRun() {
	if (_isRunning) {
		float dist = _level->tileManager->collisionDiffRight(this->getBoundingBox());
		if (dist > 0.01) {
			dist = (dist < _runSpeed) ? dist : _runSpeed;
			_level->moveLevelAtSpeed(dist);
		}
	}
}

void Player::_checkJump() {
	_upForce = fmax(_upForce - _gravity, 0);

	if (_upForce > 0.01) {// as long as jump force is stronger than gravity
		float air = _level->tileManager->collisionDiffTop(this->getBoundingBox());
		if (air > 0.01) {
			float deltaY = _maxDeltaY * (_upForce / _jumpPower);
			if (air < deltaY)
				deltaY = air;
			this->setPositionY(this->getPositionY() + deltaY);
		} else {
			_upForce = 0;
		}
		_isOnGround = false;
	} else {
		float height = _level->tileManager->collisionDiffBottom(this->getBoundingBox());
		if (height > 0.01) {
			float y = this->getPositionY();
			y -= (height < _gravity) ? height : _gravity;
			this->setPositionY(y);
			_isOnGround = false; // in case Josie falls of the cliff
		} else {
			_isOnGround = true;
		}
	}
}

void Player::_checkAlive() {
	if (this->getPositionY() < -200) {
		// KAABUUUUMMM! #splash
		this->setPosition(Vec2(216, 512));
		_level->resetLevelPosition();
	}
}

