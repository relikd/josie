#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
class Player;
class PlayerControl;
class AudioUnit;

using namespace cocos2d;

class Level : public cocos2d::Layer
{
public:
	Level();
	~Level();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(int level, int sublevel);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void pause(cocos2d::Ref* pSender);

	void addTilemap();
	void addBackground();
	void addAudio();
	void addPlayer();
	void addPlayerControl();
	void addPauseButton();
	// implement the "static create()" method manually
	CREATE_FUNC(Level);

	Player *player;
	PlayerControl *playerControl;
	AudioUnit *audioUnit;
private:
	Size visibleSize;
	Vec2 origin;
};

#endif 
