#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
class Player;
class PlayerBoss;
class PlayerControl;
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
	bool isBossLevel();

	void addTilemap();
	void addBackground();
	void addAudio();
	void addPlayer();
	void addPlayerControl();
	void addPauseButton();


	// implement the "static create()" method manually
	//CREATE_FUNC(Level);

	Player *player;
	PlayerBoss *playerBoss;
	PlayerControl *playerControl;
	AudioUnit *audioUnit;
	MapController *tileManager;
	int currentLevel;
	int currentSubLevel;
private:
	Size visibleSize;
	Vec2 origin;
};

#endif 
