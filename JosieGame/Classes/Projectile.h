#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "cocos2d.h"
class BossLevel;

class Projectile : public cocos2d::Sprite
{
public:
	Projectile();
	virtual ~Projectile();
	static Projectile* shoot(cocos2d::Vec2 start_pos, float end_x, BossLevel* level);

	bool hasCollision(Sprite* target);
	void killProjectile();
private:
	void update(float dt);
	BossLevel* _level;
};

#endif /* PROJECTILE_H_ */
