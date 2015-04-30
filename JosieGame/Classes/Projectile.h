#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "cocos2d.h"

class Projectile : public cocos2d::Sprite
{
public:
	Projectile();
	virtual ~Projectile();
	static Projectile* init(float x, float y);

	void killProjectile();
private:
	void update(float dt);
};

#endif /* PROJECTILE_H_ */
