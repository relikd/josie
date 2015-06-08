#include <cmath>
#include "Player.h"
#include "Level.h"
#include "AudioUnit.h"
#include "MapController.h"

using namespace cocos2d;

#define PLAYER_SCALE_DEFAULT 0.55
#define PLAYER_RUN_SPEED 7.0

const float _gravity = 9.81;
const float _jumpPower = 300;
const float _maxDeltaY = 20; // pixel

float _upForce; // continuously changed during jump
float _timeDiff = 0.0;

Player::Player() {
	_level = nullptr;
	_upForce = 0;
	_isRunning = false;
	_isSliding = false;
	_isOnGround = false;
	_shouldPerformDownJumpAnimation = false;
}
Player::~Player() {
	this->unscheduleUpdate();
	CCLOG("~Player");
}

Player* Player::initWithLevel(Level* level) {
	Player *pl = new Player();
	if (pl->initCollisionSize(160,245))
	{
		pl->autorelease();
		pl->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		pl->setScale(PLAYER_SCALE_DEFAULT);
		pl->_level = level;

		pl->insertImageFrameName("josiestartmove0000", Vec2(pl->getContentSize().width/2-5, -5), Vec2::ANCHOR_MIDDLE_BOTTOM);
		pl->scheduleUpdate();
	}

	return pl;
}

void Player::setPlayerOnGround(float pos_x) {
	this->setPosition(pos_x,1000);
	float height = _level->tileManager->collisionDiffBottom(this->getBoundingBox());
	this->setPositionY(1000-height);
}

void Player::onEnterTransitionDidFinish()
{

}


//
// Animation
//

Animate* Player::animationWithFrame(const std::string& name, int frameCount, float delay)
{
	Vector<SpriteFrame *> frames;
	SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();

	char file[50] = { 0 };

	for (int i = 0; i < frameCount; i++) {
		sprintf(file, "%s%04d", name.c_str(), i);
		SpriteFrame *frame = frameCache->getSpriteFrameByName(file);
		frames.pushBack(frame);
	}

	Animation *animation = Animation::createWithSpriteFrames(frames, delay);
	Animate *animate = Animate::create(animation);
	return animate;
}

void Player::startRunningAfterAnimation(FiniteTimeAction *animation)
{
	CallFuncN *call = CallFuncN::create(CC_CALLBACK_0(Player::startRunningCallback, this));
	Sequence *seq = Sequence::createWithTwoActions(animation, call);
	spriteImage->runAction(seq);
}

void Player::startRunningCallback()
{
	spriteImage->stopAllActions();
	spriteImage->runAction(RepeatForever::create(animationWithFrame("josiewalk", 6)));
}

void Player::endRunning()
{
	spriteImage->stopAllActions();
	_level->audioUnit->playJosieStopRunSound();
	spriteImage->runAction(animationWithFrame("josiestartmove", 4, 0.01)->reverse());
}


//
// Player interaction
//

void Player::run(bool r) {
	if (_isRunning == r || !_isOnGround)
		return; // only update on state change

	_isRunning = r;

	if (_isRunning)
		startRunningAfterAnimation(animationWithFrame("josiestartmove", 4, 0.01));
	else
		endRunning();
}

void Player::jump() {
	if (_isOnGround && !_isSliding) {
		_upForce = _jumpPower;
		_shouldPerformDownJumpAnimation = true;
		_level->audioUnit->playJosieJumpSound();

		spriteImage->stopAllActions();
		spriteImage->runAction(animationWithFrame("josiejump", 6, 0.01));
	}
}

void Player::slide(bool s) {
	if (_isSliding == s)
		return; // don't update while sliding
	if (_isSliding && !this->_canStandUp())
		return; // keep sliding

	_isSliding = s;

	if (_isSliding) {
		this->runAction(ScaleTo::create(0.1, PLAYER_SCALE_DEFAULT/2));
		_level->audioUnit->playJosieSlideSound();
	} else {
		this->runAction(ScaleTo::create(0.1, PLAYER_SCALE_DEFAULT));
	}
}


//
// private functions
//

void Player::update(float dt) {
	this->_checkRun();
	this->_checkJump();
	_timeDiff += dt;
	if (_timeDiff > 0.33f) {
		_timeDiff = 0.0;
		_level->tileManager->tryCollect(this->getBoundingBox());
		this->_checkAlive();
	}
}

bool Player::_canStandUp() {
	if (!_isSliding)
		return true; // already standing

	float oldScale = this->getScale();
	this->setScale(PLAYER_SCALE_DEFAULT);
	float air = _level->tileManager->collisionDiffTop(this->getBoundingBox());
	this->setScale(oldScale);

	return (air>0.01);
}

void Player::_checkRun() {
	if (_isRunning) {
		float dist = _level->tileManager->collisionDiffRight(this->getBoundingBox());
		dist = (dist < PLAYER_RUN_SPEED) ? dist : PLAYER_RUN_SPEED;
		_level->moveLevelAtSpeed(dist);
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
		if (height > 0.1) {
			float y = this->getPositionY();
			y -= (height < _gravity) ? height : _gravity;
			this->setPositionY(y);
			_isOnGround = false; // in case Josie falls of the cliff

			if (height < 3*_gravity && _shouldPerformDownJumpAnimation) {
				_shouldPerformDownJumpAnimation = false;
				startRunningAfterAnimation(animationWithFrame("josiejump", 6, 0.0001)->reverse());
			}

		} else {
			_isOnGround = true;
		}
	}
}

void Player::_checkAlive() {
	if (this->getPositionY() < -100) {
		// KAABUUUUMMM! #splash
		this->setPlayerOnGround(216);
		_level->resetLevelPosition();
	}
}

