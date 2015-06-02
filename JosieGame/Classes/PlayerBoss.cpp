#include "PlayerBoss.h"
#include "Projectile.h"
#include "BossLevel.h"

using namespace cocos2d;

const float walkspeed = 22.0;
const float frequency = 1.0;

PlayerBoss::PlayerBoss() {
	_level = NULL;
}
PlayerBoss::~PlayerBoss() {
	CCLOG("~PlayerBoss");
}

PlayerBoss* PlayerBoss::initWithLevel(BossLevel* level) {
	PlayerBoss *bp = new PlayerBoss();
	bp->initWithFile("josie/josie_transformed_static.png");
	bp->autorelease();
	bp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	bp->_level = level;

	return bp;
}

void PlayerBoss::moveLeft() {
	this->setPositionX(this->getPositionX() - walkspeed);
}

void PlayerBoss::moveRight() {
	this->setPositionX(this->getPositionX() + walkspeed);
}

bool PlayerBoss::shoot(float timeSinceLastShot)
{
	if (timeSinceLastShot >= frequency) {
		this->useShot(1);
		return true;
	}
	return false;
}

void PlayerBoss::useShot(int id)
{
	Vec2 pos = Vec2(this->getPositionX(), this->getBoundingBox().getMaxY());

	if (id&1) { // singleshot
		Projectile::shoot(pos, this->getPositionX(), _level);
	}
	else // doubleshot
	{
		pos.x -= 20;
		Projectile::shoot(pos, this->getPositionX() - 20, _level);
		pos.x += 40;
		Projectile::shoot(pos, this->getPositionX() + 20, _level);
		pos.x -= 20;
	}

	if (id>2) { // spreadshot
		pos.x -= 40;
		Projectile::shoot(pos, this->getPositionX() - 300, _level);
		pos.x += 80;
		Projectile::shoot(pos, this->getPositionX() + 300, _level);
	}
}
