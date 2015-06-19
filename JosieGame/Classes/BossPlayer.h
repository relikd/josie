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

	cocos2d::Vector<Projectile*> projectiles;
private:
	BossLevel* _level;
	float _timeSinceLastShot;

	void moveLeft();
	void moveRight();
	void shoot();
	void loadGun();

	void update(float dt);
	void registerObserver(bool reg=true);
};

#endif /* BOSSPLAYER_H_ */
