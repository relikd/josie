#include "StageHazard.h"
#include "LevelPlayer.h"

StageHazard::StageHazard() {
	_target = nullptr;
	collisionType = CollisionLayerTypeStageHazard;
}
StageHazard::~StageHazard() {
	CCLOG("~StageHazard");
}

StageHazard* StageHazard::createAt(const std::string& filename, Vec2 position, LevelPlayer* target) {
	StageHazard* other = new StageHazard();

	if (other->initCollisionSize(72, 72)) {
		other->autorelease();
		other->_initialPosition = position;
		other->_target = target;
		other->setPosition(position);
		other->insertImageName("particles/std_bullet.png", other->getContentSize() / 2);
		other->fallDown();
		other->scheduleUpdate();
	}

	return other;
}
void StageHazard::fallDown() {
	float movespeed = 1.5f/(1080/_initialPosition.y);
	MoveTo * fall = MoveTo::create(movespeed, Vec2(_initialPosition.x, -10));

	this->runAction(fall);
	CCLOG("FALLING)");
}

void StageHazard::reset() {
	this->setPosition(_initialPosition);
	this->fallDown();
}

void StageHazard::update(float dt) {
	if (this->getCollision(_target))
		_target->killPlayer();
	if (this->getPosition().y < 0) {

		this->reset();

	}
}
