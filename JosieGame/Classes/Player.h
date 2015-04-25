#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include <chrono>
class Level;
class AudioUnit;



class Player : public cocos2d::Sprite
{
public:
	Player();
	~Player();
	static Player* initWithLevel(Level* level, AudioUnit *audio);

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
	AudioUnit *_audioUnit;
	std::chrono::steady_clock::time_point _slideTime;
};

#endif // _PLAYER_H_
