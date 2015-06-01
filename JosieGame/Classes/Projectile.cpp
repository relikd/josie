#include "Projectile.h"
#include "PlayerBoss.h"

using namespace cocos2d;

Projectile::Projectile() {}
Projectile::~Projectile() {
	CCLOG("Projectile destroyed");
}

Projectile* Projectile::init(float x , float y, PlayerBoss* playerboss)
{
	Projectile* pr = new Projectile();
	pr->initWithFile("particles/std_bullet.png");
	pr->setPosition(x,y);
	pr->autorelease();
	pr->_playerboss = playerboss;
	pr->scheduleUpdate();

	return pr;
}

bool Projectile::checkCollision(Sprite* target)
{
	if(this->getBoundingBox().intersectsRect(target->getBoundingBox()))
	{
		this->killProjectile();
		return true;
	}
	return false;
}

void Projectile::update(float dt)
{
	if (this->getPositionY() >= 1100) this->killProjectile();
}

void Projectile::killProjectile()
{
	_playerboss->projectiles.eraseObject(this);
	this->removeFromParent();
}


