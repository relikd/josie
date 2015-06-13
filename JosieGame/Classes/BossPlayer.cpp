#include "BossPlayer.h"

#include "Projectile.h"
#include "BossLevel.h"

using namespace cocos2d;

#define DEFAULT_BOSSPLAYER_SCALE 0.7
#define BOSSLEVEL_BOUNDS_LEFT 154
#define BOSSLEVEL_BOUNDS_RIGHT 1766

const float walkspeed = 22.0;

BossPlayer::BossPlayer() {
	_level = nullptr;
	_timeSinceLastShot = 0;
	registerObserver();
}
BossPlayer::~BossPlayer() {
	CCLOG("~PlayerBoss");
	registerObserver(false);
}

BossPlayer* BossPlayer::createWithLevel(BossLevel* level) {

	BossPlayer *bp = new BossPlayer();
	if (bp->initCollisionSize(154,185)) {
		bp->autorelease();
		bp->_level = level;
		bp->insertImageName("josie/josie_transformed_static.png", Vec2(bp->getContentSize().width/2, 0), Vec2::ANCHOR_MIDDLE_BOTTOM);
		bp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		bp->setScale(DEFAULT_BOSSPLAYER_SCALE);
		bp->scheduleUpdate();
	}
	return bp;
}


//
// Player Controls
//

void BossPlayer::moveLeft() {
	float newX = this->getPositionX() - walkspeed;
	if (newX < BOSSLEVEL_BOUNDS_LEFT) newX = BOSSLEVEL_BOUNDS_LEFT;
	this->setPositionX(newX);
}

void BossPlayer::moveRight() {
	float newX = this->getPositionX() + walkspeed;
	if (newX > BOSSLEVEL_BOUNDS_RIGHT) newX = BOSSLEVEL_BOUNDS_RIGHT;
	this->setPositionX(newX);
}

void BossPlayer::shoot()
{
	UserDefault *ud = UserDefault::getInstance();
	int frequency = ud->getIntegerForKey("josie_perk_frequency");

	if (_timeSinceLastShot <= 1.4-frequency*0.3)
		return;

	int type = ud->getIntegerForKey("josie_perk_shoot");
	Vec2 pos = Vec2(this->getBoundingBox().getMidX(), this->getBoundingBox().getMaxY());

	if (type&1) { // singleshot
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

	if (type>2) { // spreadshot
		pos.x -= 40;
		Projectile::shoot(pos, pos.x - 300, _level);
		pos.x += 80;
		Projectile::shoot(pos, pos.x + 300, _level);
	}

	_timeSinceLastShot = 0;
}


//
// Other Functionality
//

void BossPlayer::update(float dt)
{
	_timeSinceLastShot += dt;
}

void BossPlayer::registerObserver(bool reg)
{
	EventDispatcher *ed = Director::getInstance()->getEventDispatcher();
	if (reg) {
		ed->addCustomEventListener("BOSS_PLAYER_LEFT", CC_CALLBACK_0(BossPlayer::moveLeft, this));
		ed->addCustomEventListener("BOSS_PLAYER_RIGHT", CC_CALLBACK_0(BossPlayer::moveRight, this));
		ed->addCustomEventListener("BOSS_PLAYER_SHOOT", CC_CALLBACK_0(BossPlayer::shoot, this));
	} else {
		ed->removeCustomEventListeners("BOSS_PLAYER_LEFT");
		ed->removeCustomEventListeners("BOSS_PLAYER_RIGHT");
		ed->removeCustomEventListeners("BOSS_PLAYER_SHOOT");
	}
}