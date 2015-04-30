#include "Projectile.h"

using namespace cocos2d;

Projectile::Projectile() {}
Projectile::~Projectile() {
	CCLOG("Projectile destroyed");
}

Projectile* Projectile::init(float x , float y)
{
	Projectile* pr = new Projectile();
	pr->initWithFile("particles/std_bullet.png");
	pr->setPosition(x,y);
	pr->autorelease();
	pr->scheduleUpdate();

	return pr;
}

void Projectile::update(float dt)
{
	if (this->getPositionY() >= 900) this->killProjectile();
}

void Projectile::killProjectile()
{
	this->removeFromParent();
}


