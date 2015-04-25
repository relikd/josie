#include "PlayerBoss.h"

using namespace cocos2d;

const float walkspeed = 11.0;

PlayerBoss::PlayerBoss() {}
PlayerBoss::~PlayerBoss() {}

PlayerBoss* PlayerBoss::initWithLevel(Level* level)
{
	PlayerBoss *bp = new PlayerBoss();
	bp->initWithFile("josie/josie_transformed_static.png");
	bp->autorelease();
	return bp;
}

void PlayerBoss::moveLeft()
{
	this->setPositionX( this->getPositionX() - walkspeed );
}
void PlayerBoss::moveRight()
{
	this->setPositionX( this->getPositionX() + walkspeed );
}
void PlayerBoss::shoot()
{
	// TODO: shoot particle
}
void PlayerBoss::jump()
{
	// TODO: jump
}
