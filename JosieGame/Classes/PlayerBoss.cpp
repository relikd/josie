#include "PlayerBoss.h"
#include "Projectile.h"
#include "BossLevel.h"

using namespace cocos2d;

#define DEFAULT_BOSSPLAYER_SCALE 0.7

const float walkspeed = 22.0;

PlayerBoss::PlayerBoss() {
	_level = nullptr;
	_min_x = 64 + (257/2)*DEFAULT_BOSSPLAYER_SCALE;
	_max_x = 1920 - 64 - (257/2)*DEFAULT_BOSSPLAYER_SCALE;
}
PlayerBoss::~PlayerBoss() {
	CCLOG("~PlayerBoss");
}

PlayerBoss* PlayerBoss::createWithLevel(BossLevel* level) {

	PlayerBoss *bp = new PlayerBoss();
	if (bp->initWithSize(154,185)) {
		bp->_level = level;
		bp->autorelease();
		bp->insertImage("josie/josie_transformed_static.png", Vec2::ANCHOR_MIDDLE_BOTTOM, Vec2(bp->getContentSize().width/2, 0));
		bp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		bp->setScale(DEFAULT_BOSSPLAYER_SCALE);
	}
	return bp;
}

void PlayerBoss::moveLeft() {
	float newX = this->getPositionX() - walkspeed;
	if (newX < _min_x) newX = _min_x;
	this->setPositionX(newX);
}

void PlayerBoss::moveRight() {
	float newX = this->getPositionX() + walkspeed;
	if (newX > _max_x) newX = _max_x;
	this->setPositionX(newX);
}

bool PlayerBoss::shoot(float timeSinceLastShot)
{
	UserDefault* ud = UserDefault::getInstance();
	if (timeSinceLastShot >= 1.4-(ud->getIntegerForKey("josie_perk_frequency")*0.3)) {
		this->useShot(ud->getIntegerForKey("josie_perk_shoot"));
		return true;
	}
	return false;
}

void PlayerBoss::useShot(int id)
{
	Vec2 pos = Vec2(this->getBoundingBox().getMidX(), this->getBoundingBox().getMaxY());

	if (id&1) { // singleshot
		Projectile::shoot(pos, pos.x, _level);
	}
	else // doubleshot
	{
		pos.x -= 20;
		Projectile::shoot(pos, pos.x, _level);
		pos.x += 40;
		Projectile::shoot(pos, pos.x, _level);
		pos.x -= 20;
	}

	if (id>2) { // spreadshot
		pos.x -= 40;
		Projectile::shoot(pos, pos.x - 300, _level);
		pos.x += 80;
		Projectile::shoot(pos, pos.x + 300, _level);
	}
}
