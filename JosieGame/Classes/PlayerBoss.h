#ifndef PLAYERBOSS_H_
#define PLAYERBOSS_H_

#include "CollisionLayer.h"
class Projectile;
class BossLevel;


class PlayerBoss : public CollisionLayer {
public:
	PlayerBoss();
	virtual ~PlayerBoss();

	static PlayerBoss* createWithLevel(BossLevel* level);

	void moveLeft();
	void moveRight();
	bool shoot(float timeSinceLastShot);

	void useShot(int id);
	cocos2d::Vector<Projectile*> projectiles;
private:
	BossLevel* _level;
	float _min_x, _max_x;
};

#endif /* PLAYERBOSS_H_ */
