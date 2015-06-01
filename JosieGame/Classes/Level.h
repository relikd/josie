#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
class Player;
class AudioUnit;
class MapController;


using namespace cocos2d;

class Level : public cocos2d::Scene
{
public:
	Level();
	~Level();
	static Level* initWithLevel(int level, int sublevel);

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void pause(cocos2d::Ref* pSender);

	void moveLevelAtSpeed(float speed);
	void resetLevelPosition(float position = 0.0f);

	AudioUnit *audioUnit;
	MapController *tileManager;
	Layer* moveable;
	int currentLevel;
	int currentSubLevel;
private:
	void addPauseButton();
	void addBackground();
	void addTilemap();
	void addPlayer();
	void addAudio();
};

#endif 
