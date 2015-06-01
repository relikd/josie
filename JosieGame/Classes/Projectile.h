#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "cocos2d.h"
class PlayerBoss;

class Projectile : public cocos2d::Sprite
{
public:
	Projectile();
	virtual ~Projectile();
	static Projectile* init(float x, float y,PlayerBoss* playerboss);

	bool checkCollision(Sprite* target);
	void killProjectile();
private:
	void update(float dt);
	PlayerBoss* _playerboss;
};

#endif /* PROJECTILE_H_ */
