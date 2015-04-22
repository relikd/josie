#ifndef PLAYERCONTROL_H_
#define PLAYERCONTROL_H_

#include "cocos2d.h"

class PlayerControl : public cocos2d::Layer
{
public:
	PlayerControl();
	virtual ~PlayerControl();
	static PlayerControl *create();

	void addLevelControls();
	void addBossControls();
};

#endif /* PLAYERCONTROL_H_ */
