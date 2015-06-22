#include "StageHazard.h"

StageHazard::StageHazard() {
	collisionType = CollisionLayerTypeStageHazard;
}
StageHazard::~StageHazard() {}

StageHazard* StageHazard::createAt(Vec2 position)
{
	StageHazard* other = new StageHazard();

	if (other->initCollisionSize(72, 72)) {
		other->autorelease();
		other->_initialPosition = position;
		other->setPosition(position);
		other->insertImageName("particles/EvilFruit00.png", other->getContentSize() / 2);
		other->fallDown();
	}
	return other;
}

void StageHazard::fallDown()
{
	this->stopAllActions();
	float movespeed = 1.5f/(1080/_initialPosition.y);
	DelayTime* wait = DelayTime::create(0.7f);
	MoveTo* fall = MoveTo::create(movespeed, Vec2(_initialPosition.x, -10));
	CallFuncN *placeBack = CallFuncN::create(CC_CALLBACK_0(Node::setPositionY, this, _initialPosition.y));
	this->runAction(RepeatForever::create(Sequence::create(placeBack,wait,fall, nullptr)));
}

void StageHazard::hitByCollision(CollisionLayer* other)
{
	if (other->collisionType == CollisionLayerTypeLevelPlayer) {
		this->fallDown();
	}
}