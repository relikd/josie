#ifndef PLAYERCONTROL_H_
#define PLAYERCONTROL_H_

#include "cocos2d.h"
class Level;

class PlayerControl : public cocos2d::Layer
{
public:
	PlayerControl();
	virtual ~PlayerControl();
	static PlayerControl *initWithLevel(Level* level);

	void addLevelControls();
	void addBossControls();
private:
	Level* _level;
	cocos2d::EventListenerTouchOneByOne *_listenerLevel;
};

#endif /* PLAYERCONTROL_H_ */
