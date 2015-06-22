#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "CollisionLayer.h"
class Level;

class LevelPlayer: public CollisionLayer {
public:
	LevelPlayer();
	~LevelPlayer();
	static LevelPlayer* initWithLevel(Level* level);
	void setPlayerOnGround(float pos_x);

	// Player interaction
	void run(bool r);
	void jump(bool j);
	void slide(bool s);

	void killPlayer();
private:
	bool _canStandUp();
	void _checkRun();
	void _checkJump();
	void _checkAlive();
	void resetPlayerAnimations();

	virtual void onEnterTransitionDidFinish();
	void update(float dt);
	void hitByCollision(CollisionLayer* other);
	void registerObserver(bool reg=true);

	Animate* animationWithFrame(const std::string& name, int frameCount, float delay=0.01);
	void startRunningAfterAnimation(FiniteTimeAction *animation);
	void startRunningCallback();
	void endRunning();

	Level *_level;
	bool _isSliding;
	bool _isRunning;
	bool _isOnGround;
	bool _isAlive;
	bool _shouldPerformJumpAnimation;
	float _jumpHoldingTime;
	float _upForce; // continuously changed during jump

};

#endif // _PLAYER_H_
