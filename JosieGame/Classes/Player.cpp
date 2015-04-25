#include <cmath>
#include "Player.h"
#include "Level.h"
#include "AudioUnit.h"

using namespace cocos2d;
using namespace std::chrono;

const float _gravity = 9.81;
const float _jumpPower = 300;
const float _maxDeltaY = 20; // pixel
const int _slideDuration = 1000;

float _upForce; // continuously changed during jump
float _jumpHeight;
bool _isRunning;

bool _currentlySliding;


Player::Player() {
	this->_level = NULL;
}
Player::~Player() {
	CCLOG("Player destroyed");
}

Player* Player::initWithLevel(Level* level)
{
	Player *pl = new Player();
	if (pl->initWithFile("josie/josie_static.png"))
	{
		pl->autorelease();
		pl->setAnchorPoint(Vec2(0.5, 0));
		pl->_level = level;
		_upForce=0;
		_jumpHeight=0;
		_isRunning=true;
		_currentlySliding = false;
	}
	return pl;
}

void Player::update(float dt)
{
	this->_checkRun();
	this->_checkJump();
	this->_checkSlide();
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

void Player::stopRun()
{
	_isRunning=false;
	_level->audioUnit->playJosieStopRunSound();
}

void Player::continueRun()
{
	_isRunning=true;
}

void Player::jump()
{

	if (_jumpHeight < 0.01) {
		_upForce=_jumpPower;
		_level->audioUnit->playJosieJumpSound();
	}
}

void Player::slide()
{
	auto scaleTo = ScaleTo::create(0.1, 1, 0.5); // scale y by half in 0.1sec
	auto skewTo = SkewTo::create(0.1, -30, 0);
	this->runAction(scaleTo);
	this->runAction(skewTo);

	this->_slideTime = steady_clock::now();
	_currentlySliding = true;
	_level->audioUnit->playJosieSlideSound();
}


//
// private functions
//

void Player::_unslide()
{
	_currentlySliding = false;

	auto scaleTo = ScaleTo::create(0.1, 1, 1);
	auto skewTo = SkewTo::create(0.1, 0, 0);
	this->runAction(scaleTo);
	this->runAction(skewTo);
}

void Player::_checkRun()
{
	// TODO: collision detection
	if (_isRunning || _jumpHeight > 0.0) {
		float screenWidth = Director::getInstance()->getVisibleSize().width;
		float newX = this->getPositionX()+7;
		if (newX > screenWidth) newX-=screenWidth;
		this->setPositionX(newX);
	}
}

void Player::_checkJump()
{
	_upForce = fmax(_upForce - _gravity, 0);
	float deltaY = _maxDeltaY*(_upForce/_jumpPower);
	float oldY = this->getPositionY();

	if (_upForce > 0.01) // as long as jump force is stronger than gravity
	{
		_jumpHeight+=deltaY;
		this->setPositionY(oldY+deltaY);
	}
	else if (_jumpHeight > 0.0) // gravity is decreasing height continuously
	{
		//float delY =_maxDeltaY*(_gravity/_jumpHeight);
		float newHeight = _jumpHeight-_gravity;
		_jumpHeight=fmax(newHeight,0);
		float newPosition = oldY-_gravity;
		if (newHeight < 0.0) newPosition-=newHeight; //correct height
		this->setPositionY(newPosition);
	}
}

void Player::_checkSlide()
{
	if (_currentlySliding) {
	    auto millis = duration_cast<std::chrono::milliseconds>(steady_clock::now()-this->_slideTime).count();
		if (millis > _slideDuration) {
			this->_unslide();
		}
	}
}

