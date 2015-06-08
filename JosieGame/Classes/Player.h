#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "CollisionLayer.h"
class Level;

class Player: public CollisionLayer {
public:
	Player();
	~Player();
	static Player* initWithLevel(Level* level);
	void setPlayerOnGround(float pos_x);

	// Player interaction
	void run(bool r);
	void jump();
	void slide(bool s);
private:
	void update(float dt);
	bool _canStandUp();
	void _checkRun();
	void _checkJump();
	void _checkAlive();

	virtual void onEnterTransitionDidFinish();

	Animate* animationWithFrame(const std::string& name, int frameCount, float delay=0.1);
	void startRunningAfterAnimation(FiniteTimeAction *animation);
	void startRunningCallback();
	void endRunning();

	Level *_level;
	bool _isSliding;
	bool _isRunning;
	bool _isOnGround;
	bool _shouldPerformDownJumpAnimation;
};

#endif // _PLAYER_H_
