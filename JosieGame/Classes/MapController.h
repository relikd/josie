#ifndef MAPCONTROLLER_H_
#define MAPCONTROLLER_H_

#include "cocos2d.h"
class Level;

typedef enum TilePropertyType {
	TilePropertyNone = 0,
	TilePropertyCollision,
	TilePropertyCollectable
}_TilePropertyType;

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

	TilePropertyType getTileProperty(cocos2d::Point position); // currently not used
	void collectAt(cocos2d::Point position);
private:
	cocos2d::Point getTileAt(cocos2d::Point position);
	struct TilePointOffset getTilePointOffset(cocos2d::Point point);
	void initCollisionMap();
	int getGIDForCollision();
	long getColumnBitmapForGID(int x, int tile_gid);
};

#endif /* MAPCONTROLLER_H_ */
