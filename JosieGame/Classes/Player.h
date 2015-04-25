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
	void stopRun();
	void continueRun();
	void jump();
	void slide();
	void unslide();
private:
	void _checkRun();
	void _checkJump();
	Level *_level;
	cocos2d::RepeatForever* moving();
};

#endif // _PLAYER_H_
