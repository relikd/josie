#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include "Level.h"

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
private:
	void _unslide();
	void _checkRun();
	void _checkJump();
	void _checkSlide();
	Level *_level;
};

#endif // _PLAYER_H_
