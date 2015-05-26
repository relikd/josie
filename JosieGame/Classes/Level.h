#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
class Player;
class PlayerBoss;
class Boss;
class AudioUnit;
class MapController;
class HUDLayer;


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
	bool isBossLevel();

	void moveLevelAtSpeed(float speed);
	void resetLevelPosition(float position = 0.0f);

	Player *player;
	PlayerBoss *playerBoss;
	Boss *boss;
	AudioUnit *audioUnit;
	MapController *tileManager;
	HUDLayer* HUD;
	Layer* moveable;
	int currentLevel;
	int currentSubLevel;
private:
	void addTilemap();
	void addAudio();
	void addHUD();
	void addPlayer();
};

#endif 
