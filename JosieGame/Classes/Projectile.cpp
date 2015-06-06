#include "Projectile.h"
#include "BossLevel.h"

using namespace cocos2d;

Projectile::Projectile() : _level(nullptr) {}
Projectile::~Projectile() {
	CCLOG("~Projectile");
}

Projectile* Projectile::shoot(Vec2 start_pos, float end_x, BossLevel* level)
{
	Projectile* pr = new Projectile();
	if (pr->initCollisionSize(26,26))
	{
		pr->setPosition(start_pos);
		pr->_level = level;
		pr->insertImageName("particles/std_bullet.png", pr->getContentSize()/2);

		level->addChild(pr);
		level->projectiles.pushBack(pr);

		pr->runAction(MoveTo::create(2.0f, Vec2(end_x, 1100)));
		pr->scheduleUpdate();
	}

	return pr;
}

void Projectile::update(float dt)
{
	if (this->getPositionY() >= 1100)
		this->killProjectile();
}

void Projectile::killProjectile(bool enemyHit)
{
	_level->projectiles.eraseObject(this);
	this->removeFromParent();

	if (enemyHit) {
		// fanzy animation
	}
}


