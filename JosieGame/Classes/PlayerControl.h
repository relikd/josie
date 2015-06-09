#ifndef PLAYERCONTROL_H_
#define PLAYERCONTROL_H_

#include "cocos2d.h"
class Player;
class PlayerBoss;

class PlayerControl : public cocos2d::Layer
{
public:
	PlayerControl();
	virtual ~PlayerControl();
	static PlayerControl *initWithPlayer(Player* p);
	static PlayerControl *initWithBossPlayer(PlayerBoss* p);
private:
	Player* _player;
	PlayerBoss* _bossplayer;
	float _timeSinceLastShot;
	float _timeHoldingJump;

	// Boss buttons
	cocos2d::MenuItemImage *_left;
	cocos2d::MenuItemImage *_right;
	cocos2d::MenuItemImage *_shoot;
	// Level buttons
	cocos2d::MenuItemImage *_stay;
	cocos2d::MenuItemImage *_slide;
	cocos2d::MenuItemImage *_jump;

	void addLevelControls();
	void addBossControls();
	void update(float dt);

	void jumpCallback();
};

#endif /* PLAYERCONTROL_H_ */
