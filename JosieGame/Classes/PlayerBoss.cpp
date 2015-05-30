#include "PlayerBoss.h"
#include "Projectile.h"
#include "Level.h"

using namespace cocos2d;

const float walkspeed = 11.0;

PlayerBoss::PlayerBoss() {
	_level = NULL;
}
PlayerBoss::~PlayerBoss() {
	CCLOG("~BOSSPLAYER");
}

PlayerBoss* PlayerBoss::initWithLevel(Level* level) {
	PlayerBoss *bp = new PlayerBoss();
	bp->initWithFile("josie/josie_transformed_static.png");
	bp->autorelease();
	bp->setAnchorPoint(Vec2(0.5, 0));
	bp->_level = level;
	return bp;
}

void PlayerBoss::moveLeft() {
	this->setPositionX(this->getPositionX() - walkspeed);
}
void PlayerBoss::moveRight() {
	this->setPositionX(this->getPositionX() + walkspeed);
}
void PlayerBoss::shoot(int counterForShoot) {
	if (counterForShoot == 0) {
	this->useShot(4);
	}
}
void PlayerBoss::jump() {
	// TODO: jump
}

void PlayerBoss::useShot(int id) {
	switch (id) {
	case 1: {
		Projectile* projectile_mid = Projectile::init(
					this->getBoundingBox().getMidX(),
					this->getBoundingBox().getMaxY());
		_level->addChild(projectile_mid, 1);
		projectile_mid->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX(), 1100)));
	} break;
	case 2: {
		Projectile* projectile_midright = Projectile::init(
				this->getBoundingBox().getMidX() + 20,
				this->getBoundingBox().getMaxY());
		Projectile* projectile_midleft = Projectile::init(
				this->getBoundingBox().getMidX() - 20,
				this->getBoundingBox().getMaxY());
		_level->addChild(projectile_midleft, 1);
		_level->addChild(projectile_midright, 1);
		projectile_midleft->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX() - 20, 1100)));
		projectile_midright->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX() + 20, 1100)));
	}break;
	case 3: {
		Projectile* projectile_mid = Projectile::init(
				this->getBoundingBox().getMidX(),
				this->getBoundingBox().getMaxY());
		Projectile* projectile_left = Projectile::init(
				this->getBoundingBox().getMidX() - 40,
				this->getBoundingBox().getMaxY());
		Projectile* projectile_right = Projectile::init(
				this->getBoundingBox().getMidX() + 40,
				this->getBoundingBox().getMaxY());
		_level->addChild(projectile_mid, 1);
		_level->addChild(projectile_left, 1);
		_level->addChild(projectile_right, 1);
		projectile_mid->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX(), 1100)));
		projectile_left->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX() - 300, 1100)));
		projectile_right->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX() + 300, 1100)));
	}break;
	case 4: {
		Projectile* projectile_midright = Projectile::init(
				this->getBoundingBox().getMidX() + 20,
				this->getBoundingBox().getMaxY());
		Projectile* projectile_midleft = Projectile::init(
				this->getBoundingBox().getMidX() - 20,
				this->getBoundingBox().getMaxY());
		Projectile* projectile_left = Projectile::init(
				this->getBoundingBox().getMidX() - 40,
				this->getBoundingBox().getMaxY());
		Projectile* projectile_right = Projectile::init(
				this->getBoundingBox().getMidX() + 40,
				this->getBoundingBox().getMaxY());
		_level->addChild(projectile_midleft, 1);
		_level->addChild(projectile_midright, 1);
		_level->addChild(projectile_left, 1);
		_level->addChild(projectile_right, 1);
		projectile_midleft->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX() - 20, 1100)));
		projectile_midright->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX() + 20, 1100)));
		projectile_left->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX() - 300, 1100)));
		projectile_right->runAction(
				MoveTo::create(2.0f, Vec2(this->getPositionX() + 300, 1100)));
	}break;
	}
}
