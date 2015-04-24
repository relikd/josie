#ifndef PLAYERCONTROL_H_
#define PLAYERCONTROL_H_

#include "cocos2d.h"
#include "Player.h"

class PlayerControl : public cocos2d::Layer
{
public:
	PlayerControl();
	virtual ~PlayerControl();
	static PlayerControl *initWithPlayer(Player* player);

	void addLevelControls();
	void addBossControls();
private:
	Player* _player;
};

#endif /* PLAYERCONTROL_H_ */
