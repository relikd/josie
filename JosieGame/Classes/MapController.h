#ifndef MAPCONTROLLER_H_
#define MAPCONTROLLER_H_

typedef struct IntPoint { int x; int y; }_IntPoint;
#define COINS_PER_LEVEL 20

#include "cocos2d.h"

class CollisionLayer;
class Player;

class MapController : public cocos2d::TMXTiledMap {
public:
	MapController();
	~MapController();

	static MapController* initWithLevel(int level, int sub_level);
	void initOptions();

	bool tryCollect(Player *player);

	float collisionDiffTop(cocos2d::Rect bounds);
	float collisionDiffBottom(cocos2d::Rect bounds);
	float collisionDiffRight(cocos2d::Rect bounds);

	void reinitializeMap();
private:
	long *_collisionMap;
	cocos2d::Vector<CollisionLayer*> _coins;

	void initCollectableArray();
	void initCollisionMap();

	cocos2d::Point coordinateFromTilePoint(cocos2d::Point tileCoord);
	struct TilePointOffset getTilePointOffset(cocos2d::Point point);
	long getColumnBitmapForGID(int x, int tile_gid);
	int getGIDForName(const std::string& name);
};

#endif /* MAPCONTROLLER_H_ */
