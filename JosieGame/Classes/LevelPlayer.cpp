#include "LevelPlayer.h"

#include "Level.h"
#include "AudioUnit.h"
#include "MapController.h"

using namespace cocos2d;

#define PLAYER_SCALE_DEFAULT 0.55
#define PLAYER_RUN_SPEED 7.0

const float _gravity = 9.81;
const float _jumpPower = 200;

float _upForce; // continuously changed during jump

LevelPlayer::LevelPlayer() {
	_level = nullptr;
	_upForce = 0;
	_isRunning = false;
	_isSliding = false;
	_isOnGround = false;
	_shouldPerformJumpAnimation = false;
	_oldJumpHoldingTime = 999;
	registerObserver();
}
LevelPlayer::~LevelPlayer() {
	registerObserver(false);
	CCLOG("~Player");
}

LevelPlayer* LevelPlayer::initWithLevel(Level* level) {
	LevelPlayer *pl = new LevelPlayer();
	if (pl->initCollisionSize(160,245))
	{
		pl->autorelease();
		pl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		pl->setScale(PLAYER_SCALE_DEFAULT);
		pl->_level = level;

		pl->insertImageFrameName("josiestartmove0000", Vec2(pl->getContentSize().width/2-5, -5), Vec2::ANCHOR_MIDDLE_BOTTOM);
		pl->scheduleUpdate();
	}

	return pl;
}

void LevelPlayer::setPlayerOnGround(float pos_x) {
	this->setPosition(pos_x,1000);
	float height = _level->mapManager->collisionDiffBottom(this->getBoundingBox());
	this->setPositionY(1000-height);

	resetPlayerAnimations();
}

void LevelPlayer::resetPlayerAnimations()
{
	_isOnGround = true;
	_isRunning = false;
	_isSliding = false;
	this->setScale(PLAYER_SCALE_DEFAULT);
	this->spriteImage->stopAllActions();
	this->spriteImage->setSpriteFrame("josiestartmove0000");
}

void LevelPlayer::onEnterTransitionDidFinish()
{

}


//
// Animation
//

Animate* LevelPlayer::animationWithFrame(const std::string& name, int frameCount, float delay)
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

void LevelPlayer::startRunningAfterAnimation(FiniteTimeAction *animation)
{
	CallFuncN *call = CallFuncN::create(CC_CALLBACK_0(LevelPlayer::startRunningCallback, this));
	Sequence *seq = Sequence::createWithTwoActions(animation, call);
	spriteImage->runAction(seq);
}

void LevelPlayer::startRunningCallback()
{
	spriteImage->stopAllActions();
	spriteImage->runAction(RepeatForever::create(animationWithFrame("josiewalk", 6, 0.1)));
}

void LevelPlayer::endRunning()
{
	spriteImage->stopAllActions();
	AudioUnit::playJosieStopRunSound();
	spriteImage->runAction(animationWithFrame("josiestartmove", 4)->reverse());
}


//
// Player interaction
//

void LevelPlayer::run(bool r) {
	if (_isRunning == r || !_isOnGround)
		return; // only update on state change

	_isRunning = r;

	if (_isRunning)
		startRunningAfterAnimation(animationWithFrame("josiestartmove", 4));
	else
		endRunning();
}

void LevelPlayer::jump() {
	if (!_isSliding) {
		if (_isOnGround && _shouldPerformJumpAnimation == false)
		{
			_oldJumpHoldingTime = 0;
			_shouldPerformJumpAnimation = true;
			AudioUnit::playJosieJumpSound();

			spriteImage->stopAllActions();
			spriteImage->runAction(animationWithFrame("josiejump", 6));
		}

		if (_oldJumpHoldingTime <= 0.2) {
			_upForce = 50 + _jumpPower * (_oldJumpHoldingTime / 0.2); // longest duration hold = 0.2sec
		} else {
			_oldJumpHoldingTime = 999;
		}
	}
}

void LevelPlayer::slide(bool s) {
	if (_isSliding == s)
		return; // don't update while sliding
	if (_isSliding && !this->_canStandUp())
		return; // keep sliding

	_isSliding = s;

	if (_isSliding) {
		this->runAction(ScaleTo::create(0.1, PLAYER_SCALE_DEFAULT/2));
		AudioUnit::playJosieSlideSound();
	} else {
		this->runAction(ScaleTo::create(0.1, PLAYER_SCALE_DEFAULT));
	}
}


//
// private functions
//

void LevelPlayer::update(float dt)
{
	_oldJumpHoldingTime += dt;

	this->_checkRun();
	this->_checkJump();
	this->_checkAlive();

	if (_level->mapManager->tryCollect(this))
		_level->addCoin();
}

bool LevelPlayer::_canStandUp() {
	if (!_isSliding)
		return true; // already standing

	float oldScale = this->getScale();
	this->setScale(PLAYER_SCALE_DEFAULT);
	float air = _level->mapManager->collisionDiffTop(this->getBoundingBox());
	this->setScale(oldScale);

	return (air>0.01);
}

void LevelPlayer::_checkRun() {
	if (_isRunning) {
		float dist = _level->mapManager->collisionDiffRight(this->getBoundingBox());
		dist = (dist < PLAYER_RUN_SPEED) ? dist : PLAYER_RUN_SPEED;
		this->setPositionX( this->getPositionX() + dist );
		_level->moveLevelAtSpeed(dist);
	}
}

void LevelPlayer::_checkJump() {
	_upForce = fmax(_upForce - _gravity, 0);

	if (_upForce > 0.01) // as long as jump force is stronger than gravity
	{
		float air = _level->mapManager->collisionDiffTop(this->getBoundingBox());
		if (air > 0.01) {
			float deltaY = 20 * (_upForce / _jumpPower);
			if (air < deltaY)
				deltaY = air;
			this->setPositionY(this->getPositionY() + deltaY);
		} else {
			_upForce = 0;
		}
		_isOnGround = false;
	}
	else
	{
		float height = _level->mapManager->collisionDiffBottom(this->getBoundingBox());
		float y = this->getPositionY();
		y -= (height < 2*_gravity) ? height : 2*_gravity;
		this->setPositionY(y);

		_isOnGround = (height < 0.1); // in case Josie falls of the cliff

		if (_shouldPerformJumpAnimation && height < 3*_gravity) {
			_shouldPerformJumpAnimation = false;
			startRunningAfterAnimation(animationWithFrame("josiejump", 6, 0.0001)->reverse());
		}
	}
}

void LevelPlayer::_checkAlive() {
	if (this->getPositionY() < -100) {
		// KAABUUUUMMM! #splash
		if (100.0 <= _level->mapManager->getLevelProgress(this->getBoundingBox()))
			_level->finishLevelSuccessfull();
		else
			_level->finishLevelSuccessfull(false);

		this->setPlayerOnGround(400);
	}
}

void LevelPlayer::registerObserver(bool reg)
{
	EventDispatcher *ed = Director::getInstance()->getEventDispatcher();
	if (reg) {
		ed->addCustomEventListener("LEVEL_PLAYER_SLIDE_0", CC_CALLBACK_0(LevelPlayer::slide, this, 0));
		ed->addCustomEventListener("LEVEL_PLAYER_SLIDE_1", CC_CALLBACK_0(LevelPlayer::slide, this, 1));
		ed->addCustomEventListener("LEVEL_PLAYER_RUN_0", CC_CALLBACK_0(LevelPlayer::run, this, 0));
		ed->addCustomEventListener("LEVEL_PLAYER_RUN_1", CC_CALLBACK_0(LevelPlayer::run, this, 1));
		ed->addCustomEventListener("LEVEL_PLAYER_JUMP", CC_CALLBACK_0(LevelPlayer::jump, this));
	} else {
		ed->removeCustomEventListeners("LEVEL_PLAYER_SLIDE_0");
		ed->removeCustomEventListeners("LEVEL_PLAYER_SLIDE_1");
		ed->removeCustomEventListeners("LEVEL_PLAYER_RUN_0");
		ed->removeCustomEventListeners("LEVEL_PLAYER_RUN_1");
		ed->removeCustomEventListeners("LEVEL_PLAYER_JUMP");
	}
}
