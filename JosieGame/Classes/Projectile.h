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

	void killProjectile(bool enemyHit=false);
private:
	BossLevel* _level;
};

#endif /* PROJECTILE_H_ */
