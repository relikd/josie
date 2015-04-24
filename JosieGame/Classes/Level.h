#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
class Player;
USING_NS_CC;
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
	Player* addPlayer();
	void addPlayerControl(Player* player);
	void addPauseButton();
	// implement the "static create()" method manually
	CREATE_FUNC(Level);
private:
	Size visibleSize;
	Vec2 origin;
};

#endif 
