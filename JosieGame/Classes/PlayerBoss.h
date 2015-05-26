#ifndef PLAYERBOSS_H_
#define PLAYERBOSS_H_

#include "cocos2d.h"
class Level;

class PlayerBoss : public cocos2d::Sprite {
public:
	PlayerBoss();
	virtual ~PlayerBoss();
	static PlayerBoss* initWithLevel(Level* level);

	void moveLeft();
	void moveRight();
	void shoot(int counterForShoot);
	void jump();
private:
	Level* _level;
};

#endif /* PLAYERBOSS_H_ */
