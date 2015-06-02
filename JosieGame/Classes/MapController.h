#ifndef MAPCONTROLLER_H_
#define MAPCONTROLLER_H_

typedef struct IntPoint { int x; int y; }_IntPoint;

#include "cocos2d.h"

class Level;

class MapController : public cocos2d::TMXTiledMap {
public:
	MapController();
	~MapController();

	cocos2d::TMXTiledMap *map;
	float mapOffsetX;

	static MapController* initWithLevel(Level *lvl);
	static MapController* initWithObject(TMXTiledMap* map);
	void initOptions();

	float collisionDiffTop(cocos2d::Rect bounds);
	float collisionDiffBottom(cocos2d::Rect bounds);
	float collisionDiffRight(cocos2d::Rect bounds);

	bool tryCollect(cocos2d::Rect playerBounds);
private:
	long *_collisionMap;
	IntPoint _coinArray[200];
	cocos2d::TMXLayer *_metaLayer;

	void collectAt(cocos2d::Point tileCoord);
	struct TilePointOffset getTilePointOffset(cocos2d::Point point);
	void initCollectableArray();
	void initCollisionMap();
	long getColumnBitmapForGID(int x, int tile_gid);
	int getGIDForCollision();
	int getGIDForCollectable();
};

#endif /* MAPCONTROLLER_H_ */
