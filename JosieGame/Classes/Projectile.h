#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "CollisionLayer.h"
class BossLevel;

class Projectile : public CollisionLayer
{
public:
	Projectile();
	virtual ~Projectile();
	static Projectile* shoot(Vec2 start_pos, float end_x, BossLevel* level);

	bool hasCollision(Sprite* target);
	void killProjectile(bool enemyHit);
private:
	void update(float dt);
	BossLevel* _level;
};

#endif /* PROJECTILE_H_ */
