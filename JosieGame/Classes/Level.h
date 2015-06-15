#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"

class MapController;
class LevelHUD;

class Level : public cocos2d::Scene
{
public:
	Level();
	~Level();
	static Level* initWithLevel(int level, int sublevel);

	void moveLevelAtSpeed(float speed);
	void resetLevelPosition(float position = 0.0f);
	void addCoin();

	MapController *mapManager;
	int coins;
private:
	LevelHUD *hud;
	void createUI(int lvl, int sublvl);
};

#endif 
