#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"

class AudioUnit;
class MapController;

class Level : public cocos2d::Scene
{
public:
	Level();
	~Level();
	static Level* initWithLevel(int level, int sublevel);


	void moveLevelAtSpeed(float speed);
	void resetLevelPosition(float position = 0.0f);

	AudioUnit *audioUnit;
	MapController *mapManager;
private:
	void createUI(int lvl, int sublvl);
	void addPauseButton();
	void pauseGame();
};

#endif 
