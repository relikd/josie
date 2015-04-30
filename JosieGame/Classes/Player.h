#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
class Level;

class Player: public cocos2d::Sprite {
public:
	Player();
	~Player();
	static Player* initWithLevel(Level* level);

	void update(float dt);

	// Player interaction
	void run(bool r);
	void jump();
	void slide(bool s);
private:
	bool _canStandUp();
	void _checkRun();
	void _checkJump();
	void _checkAlive();
	cocos2d::Rect addOffsetToBBox();

	cocos2d::RepeatForever* moving();

	Level *_level;
	bool _isSliding;
	bool _isRunning;
	bool _isOnGround;
	float offset;
};

#endif // _PLAYER_H_
