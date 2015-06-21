/*
 * StageHazard.cpp
 *
 *  Created on: 20.06.2015
 *      Author: Jonas
 */

#include "StageHazard.h"
#include "LevelPlayer.h"

StageHazard::StageHazard() {
	// TODO Auto-generated constructor stub
	_target = NULL;
}

StageHazard::~StageHazard() {
	// TODO Auto-generated destructor stub
}

StageHazard* StageHazard::createAt(const std::string& filename, Vec2 position, LevelPlayer* target) {
	StageHazard* other = new StageHazard();

	if (other->initCollisionSize(72, 72)) {
		other->autorelease();
		other->_Position = position;
		other->_target = target;
		other->setPosition(position);
		other->insertImageName("particles/std_bullet.png",
				other->getContentSize() / 2);
		other->fallDown();
		other->scheduleUpdate();
	}

	return other;
}
void StageHazard::fallDown() {
	float movespeed = 1.5f/(1080/_Position.y);
	MoveTo * fall = MoveTo::create(movespeed, Vec2(_Position.x, -10));

	this->runAction(fall);
	CCLOG("FALLING)");
}

void StageHazard::reset() {
	this->setPosition(_Position);
	this->fallDown();
}

void StageHazard::update(float dt) {
	if (this->getCollision(_target))
		_target->killPlayer();
	if (this->getPosition().y < 0) {

		this->reset();

	}
}
