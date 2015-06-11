#ifndef BOSSPLAYER_H_
#define BOSSPLAYER_H_

#include "CollisionLayer.h"
class Projectile;
class BossLevel;


class BossPlayer : public CollisionLayer {
public:
	BossPlayer();
	virtual ~BossPlayer();

	static BossPlayer* createWithLevel(BossLevel* level);

	void moveLeft();
	void moveRight();
	bool shoot(float timeSinceLastShot);

	void useShot(int id);
	cocos2d::Vector<Projectile*> projectiles;
private:
	BossLevel* _level;
	float _min_x, _max_x;
};

#endif /* BOSSPLAYER_H_ */
