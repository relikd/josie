#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"

class MapController;
class LevelHUD;
class LevelPlayer;

class Level : public cocos2d::Scene
{
public:
	Level();
	~Level();
	static Level* initWithLevel(int level, int sublevel);
	static Level* initWithLevelAndDifficulty(int level, int sublevel,int difficulty);

	void scheduleHUD();
	void unscheduleHUD();

	void moveLevelAtSpeed(float speed);
	void resetLevelPosition(float position = 0.0f);
	void addCoin();
	void finishLevelSuccessfull(bool successfull=true);

	MapController *mapManager;
	int coins;
private:
	int _maxCoins;
	int _level;
	int _sublevel;
	int _rnd_diff;
	LevelPlayer* _player;
	LevelHUD *hud;
	void createUI(int lvl, int sublvl);
	void startAfterDelay(float delay);
};

#endif 
