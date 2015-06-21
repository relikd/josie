#include "Projectile.h"
#include "BossLevel.h"

using namespace cocos2d;

Projectile::Projectile() {
	_level = nullptr;
	collisionType = CollisionLayerTypeProjectile;
}
Projectile::~Projectile() {
	CCLOG("~Projectile");
}

Projectile* Projectile::shoot(Vec2 start_pos, float end_x, BossLevel* level)
{
	Projectile* pr = new Projectile();
	if (pr->initCollisionSize(26,26))
	{
		pr->autorelease();
		pr->setPosition(start_pos);
		pr->_level = level;
		pr->insertImageName("particles/std_bullet.png", pr->getContentSize()/2);

		level->addChild(pr, 0);
		level->projectiles.pushBack(pr);

		MoveTo *shoot = MoveTo::create(2.0f, Vec2(end_x, 1100));
		CallFuncN *call = CallFuncN::create(CC_CALLBACK_0(Projectile::killProjectile, pr, false));
		Sequence *seq = Sequence::createWithTwoActions(shoot, call);
		pr->runAction(seq);
	}

	return pr;
}

void Projectile::killProjectile(bool enemyHit)
{
	_level->projectiles.eraseObject(this);
	this->removeFromParent();

	if (enemyHit) {
		// fanzy animation
	}
}


