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

	static MapController* initWithLevel(Level *lvl);
	void initOptions();

	TilePropertyType getTileProperty(cocos2d::Point position);
	TilePropertyType getTilePropertyFromGID(cocos2d::Point gid);
	void collectAt(cocos2d::Point position);
	bool hasCollisionBetweenPoints(cocos2d::Point a, cocos2d::Point b);
private:
	cocos2d::Point getTileAt(cocos2d::Point position);
public:
	cocos2d::TMXTiledMap *map;
};

#endif /* MAPCONTROLLER_H_ */
