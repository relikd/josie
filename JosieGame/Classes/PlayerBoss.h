#ifndef PLAYERBOSS_H_
#define PLAYERBOSS_H_

#include "cocos2d.h"
class Projectile;
class Level;

class PlayerBoss : public cocos2d::Sprite {
public:
	PlayerBoss();
	virtual ~PlayerBoss();
	static PlayerBoss* initWithLevel(Level* level);

	void moveLeft();
	void moveRight();
	void shoot(float counterForShoot);
	void jump();

	void useShot(int id);
	cocos2d::Vector<Projectile*> projectiles;
private:
	Level* _level;

};

#endif /* PLAYERBOSS_H_ */
