#include "Projectile.h"
#include "BossLevel.h"

using namespace cocos2d;

Projectile::Projectile() {
	_level = NULL;
}
Projectile::~Projectile() {
	CCLOG("~Projectile");
}

Projectile* Projectile::shoot(Vec2 start_pos, float end_x, BossLevel* level)
{
	Projectile* pr = new Projectile();
	if (pr->initWithFile("particles/std_bullet.png"))
	{
		pr->setPosition(start_pos);
		pr->_level = level;

		level->addChild(pr);
		level->projectiles.pushBack(pr);

		pr->runAction(MoveTo::create(2.0f, Vec2(end_x, 1100)));
		pr->scheduleUpdate();
	}

	return pr;
}

bool Projectile::hasCollision(Sprite* target)
{
	if(this->getBoundingBox().intersectsRect(target->getBoundingBox()))
	{
		this->killProjectile(true);
		return true;
	}
	return false;
}

void Projectile::update(float dt)
{
	if (this->getPositionY() >= 1100)
		this->killProjectile(false);
}

void Projectile::killProjectile(bool enemyHit)
{
	_level->projectiles.eraseObject(this);
	this->removeFromParent();

	if (enemyHit) {
		// fanzy animation
	}
}


