#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "CollisionLayer.h"
class Level;

class Player: public CollisionLayer {
public:
	Player();
	~Player();
	static Player* initWithLevel(Level* level);

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

	RepeatForever* animationWithFrame(const std::string& name, int frameCount);
	Sprite *_animationSprite;

	Level *_level;
	bool _isSliding;
	bool _isRunning;
	bool _isOnGround;
};

#endif // _PLAYER_H_
