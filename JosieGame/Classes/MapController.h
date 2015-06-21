#ifndef MAPCONTROLLER_H_
#define MAPCONTROLLER_H_

typedef struct IntPoint { int x; int y; }_IntPoint;

#include "cocos2d.h"

class CollisionLayer;

class MapController : public cocos2d::TMXTiledMap {
public:
	MapController();
	~MapController();

	static MapController* initWithLevel(int level, int sub_level);
	void initOptions();

	void setPlayerAsListener(CollisionLayer *player); // no need for Player class
	bool checkDeathly(cocos2d::Rect bounds);
	
	void collectCoin(CollisionLayer *coin);
	int getMaxCoins() { return _maxCoins; }
	float getLevelProgress(cocos2d::Rect bounds);

	float collisionDiffTop(cocos2d::Rect bounds);
	float collisionDiffBottom(cocos2d::Rect bounds);
	float collisionDiffRight(cocos2d::Rect bounds);

	void reinitializeMap(bool re_collision=true, bool re_interactables=true);
private:
	long *_collisionMap;
	long *_deathlyMap;
	int _maxCoins;
	cocos2d::Vector<CollisionLayer*> _interactables;

	void initInteractables();
	void initDeathlyArray();
	void initCollisionMap();

	cocos2d::Point coordinateFromTilePoint(cocos2d::Point tileCoord);
	struct TilePointOffset getTilePointOffset(cocos2d::Point point);
	long getColumnBitmapForGID(int x, int tile_gid);
	int getGIDForName(const std::string& name);
};

#endif /* MAPCONTROLLER_H_ */
