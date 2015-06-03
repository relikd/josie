#ifndef PLAYERBOSS_H_
#define PLAYERBOSS_H_

#include "cocos2d.h"
class Projectile;
class BossLevel;

class PlayerBoss : public cocos2d::Sprite {
public:
	PlayerBoss();
	virtual ~PlayerBoss();
	static PlayerBoss* initWithLevel(BossLevel* level);

	void moveLeft();
	void moveRight();
	bool shoot(float timeSinceLastShot);

	void useShot(int id);
	cocos2d::Vector<Projectile*> projectiles;
private:
	BossLevel* _level;


};

#endif /* PLAYERBOSS_H_ */
