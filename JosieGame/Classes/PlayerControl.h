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

	void update(float dt);
	void jumpCallback(cocos2d::Ref* pSender);
private:
	Level* _level;
	float _counterForShoot;
	cocos2d::EventListenerTouchOneByOne *_listenerLevel;

	// Boss buttons
	cocos2d::MenuItemImage *_left;
	cocos2d::MenuItemImage *_right;
	cocos2d::MenuItemImage *_shoot;
	// Level buttons
	cocos2d::MenuItemImage *_stay;
	cocos2d::MenuItemImage *_slide;
};

#endif /* PLAYERCONTROL_H_ */
