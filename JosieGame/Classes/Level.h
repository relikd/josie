#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
class Player;
class PlayerBoss;
class PlayerControl;
class AudioUnit;

using namespace cocos2d;

typedef enum TilePropertyType {
	TilePropertyNone = 0,
	TilePropertyCollision,
	TilePropertyCollectable
}_TilePropertyType;

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

	TilePropertyType getTileProperty(cocos2d::Point position);
	void collectAt(cocos2d::Point position);
	// implement the "static create()" method manually
	//CREATE_FUNC(Level);

	Player *player;
	PlayerBoss *playerBoss;
	PlayerControl *playerControl;
	AudioUnit *audioUnit;
	int currentLevel;
	int currentSubLevel;
private:

	cocos2d::Point getTileAt(cocos2d::Point position);
	Size visibleSize;
	Vec2 origin;


	cocos2d::TMXTiledMap *_tileMap;
	cocos2d::TMXLayer *_tilemapBackground;
	cocos2d::TMXLayer *_metaLayer;
};

#endif 
