#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include <chrono>
class Level;

class Player : public cocos2d::Sprite
{
public:
	Player();
	~Player();
	static Player* initWithLevel(Level* level);

	void update(float dt);

	// Enter & Exit Scene
	void onEnterTransitionDidFinish();
	void onExitTransitionDidStart();

	// Player interaction
	void run(bool r);
	void jump();
	void slide(bool s);
private:
	void _checkRun();
	void _checkJump();

	cocos2d::RepeatForever* moving();

	Level *_level;
	bool _isSliding;
	bool _isRunning;
};

#endif // _PLAYER_H_
