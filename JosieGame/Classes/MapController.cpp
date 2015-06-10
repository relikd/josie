#include "MapController.h"

#include "Level.h"
#include "CollisionLayer.h"
#include "Player.h"

using namespace cocos2d;

typedef struct TilePointOffset { int x; int y; float offsetX; float offsetY; }_TilePointOffset;


MapController::MapController() {
	map = NULL;
	mapOffsetX = 0.0f;
	_collisionMap = new long[1];
}
MapController::~MapController() {
	delete _collisionMap;
	CCLOG("~MapController");
}

MapController* MapController::initWithLevel(int level, int sub_level)
{
	MapController* tmc = new MapController();
	//tmc->autorelease();

	std::ostringstream mapstr;
	mapstr << "tilemaps/" << level << "." << sub_level << ".tmx";
	tmc->map = TMXTiledMap::create(mapstr.str());
	tmc->initOptions();

	return tmc;
}
//initialisiert MAp nicht aus Datei direkt sondern aus gegebenem TMXTiledMap Objekt
MapController* MapController::initWithObject(TMXTiledMap* map)
{
	MapController* tmc = new MapController();
	//tmc->autorelease();
	tmc->map = map;
	tmc->initOptions();

	return tmc;
}

void MapController::initOptions()
{
	_coins = Vector<CollisionLayer*>{COINS_PER_LEVEL+1};
	map->getLayer("Meta_layer")->setVisible(true);

	for (const auto& child : map->getChildren()) {
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
	this->initCollisionMap();
	this->initCollectableArray();
}

//
// :::::::::: Gameplay Functionality ::::::::::
//

bool MapController::tryCollect(Player *player)
{
	for(CollisionLayer* coin : this->_coins)
	{
		if (player->getCollision(coin)) {
			_coins.eraseObject(coin);
			coin->removeFromParent();
			return true;
		}
	}
	return false;
}

/*void MapController::collectAt(Point tileCoord) {
	map->getLayer("Meta_layer")->removeTileAt(tileCoord);
	map->getLayer("Foreground_layer")->removeTileAt(tileCoord);
}*/


//
// :::::::::: Collision Detection ::::::::::
//

float MapController::collisionDiffTop(Rect bounds)
{
	TilePointOffset pos = this->getTilePointOffset(Vec2(bounds.origin.x, bounds.getMaxY()));
	int numTiles = ceil((pos.offsetX + bounds.size.width) / map->getTileSize().width);
	float topDistance = 9999;
	for (int i=0; i<numTiles; i++)
	{
		long col = _collisionMap[pos.x+i];
		int distance = 9999;
		for (int u=0; u<pos.y; u++)
		{
			if (col&1) distance = 0;
			else distance += map->getTileSize().height;
			col>>=1;
		}
		if (col&1) // already in collision
			return pos.offsetY -0.1f - map->getTileSize().height;
		if (distance < topDistance) topDistance = distance;
	}
	return topDistance + pos.offsetY -0.1f;
}

float MapController::collisionDiffBottom(Rect bounds)
{
	TilePointOffset pos = this->getTilePointOffset(bounds.origin);
	int numTiles = ceil((pos.offsetX + bounds.size.width) / map->getTileSize().width);
	float bottomDistance = 9999;
	for (int i=0; i<numTiles; i++)
	{
		long col = _collisionMap[pos.x+i];
		int distance = 0;
		col >>= pos.y;
		if (col==0) continue; // infinite distance. something like a hole
		while (!(col&1)) {
			distance += map->getTileSize().height;
			col>>=1;
		}
		if (distance < bottomDistance) bottomDistance = distance;
	}
	return bottomDistance - pos.offsetY -0.1f;
}

float MapController::collisionDiffRight(Rect bounds)
{
	TilePointOffset pos = this->getTilePointOffset(Vec2(bounds.getMaxX(), bounds.getMaxY()));
	int playerHeightInTiles = ceil((pos.offsetY+bounds.size.height) / map->getTileSize().height);

	for (int i=0; i<2; i++) // get current and next column
	{
		int numTiles = playerHeightInTiles;
		long col = _collisionMap[pos.x+i];
		col >>= pos.y;
		while (numTiles--) {
			if (col&1) return i*map->getTileSize().width - pos.offsetX -0.1f;
			col>>=1;
		}
	}
	return 2*map->getTileSize().width - pos.offsetX; // collision at least one tile away
}


//
// :::::::::: Other Functionality ::::::::::
//

void MapController::initCollectableArray()
{
	int mapWidth = (int)map->getMapSize().width;
	int mapHeight = (int)map->getMapSize().height;
	int collectGID = getGIDForCollectable();

	for (int x=0; x<mapWidth; x++)
	{
		for (int y=0; y<mapHeight; y++)
		{
			if (collectGID == map->getLayer("Meta_layer")->getTileGIDAt(Vec2(x,y)))
			{
				CollisionLayer *coin = CollisionLayer::createCoinSprite();
				coin->setPosition(coordinateFromTilePoint(Vec2(x,y-1))); // 1 tile above collectable
				_coins.pushBack(coin);
				map->addChild(coin);
			}
		}
	}
}

void MapController::initCollisionMap()
{
	int mapWidth = (int)map->getMapSize().width;
	int collisionGID = this->getGIDForCollision();

	delete _collisionMap;
	_collisionMap = new long[mapWidth+10]; // remember to free space

	for (int x=0; x<mapWidth+10; x++) {
		_collisionMap[x] = 0; // just a few extra columns for collision
		if (x<mapWidth) {
			long column = this->getColumnBitmapForGID(x, collisionGID);
			_collisionMap[x] = column;
		}
	}
}


//
// :::::::::: Internal/Private Functions ::::::::::
//

// wandelt Position in Tilemap Koordinate um
/*Point MapController::getTileAt(Point position) {
	int x = (position.x + mapOffsetX) / map->getTileSize().width;
	int y = ((map->getMapSize().height * map->getTileSize().height)
			- position.y) / map->getTileSize().height;
	return Point(x, y);
}*/

Point MapController::coordinateFromTilePoint(Point tileCoord)
{
	Point pos;
	pos.x = tileCoord.x * map->getTileSize().width;
	pos.y = (map->getMapSize().height -1 -tileCoord.y) * map->getTileSize().height;
	pos += map->getTileSize()/2; // midpoint
	return pos;
}

TilePointOffset MapController::getTilePointOffset(Point point)
{
	TilePointOffset tpo;
	float y = ((map->getMapSize().height * map->getTileSize().height) - point.y);
	tpo.offsetX = fmod(point.x, map->getTileSize().width); // + mapOffsetX
	tpo.offsetY = fmod(y, map->getTileSize().height);
	tpo.x = (point.x) / map->getTileSize().width; // + mapOffsetX
	tpo.y = (y / map->getTileSize().height);
	return tpo;
}

long MapController::getColumnBitmapForGID(int x, int tile_gid)
{
	long col=0;
	for (int i=map->getMapSize().height; i>0; i--) {
		col<<=1;
		int gid = map->getLayer("Meta_layer")->getTileGIDAt(Vec2(x,i-1));
		col |= (gid==tile_gid);
	}
	return col;
}

int MapController::getGIDForCollision()
{
	for (int i=0; i<999; i++) {
		Value propMap = map->getPropertiesForGID(i);
		if (propMap.getType() == Value::Type::MAP &&
			propMap.asValueMap()["Collision"].asBool()) {
			return i;
		}
	}
	return -1;
}

int MapController::getGIDForCollectable()
{
	for (int i=0; i<999; i++) {
		Value propMap = map->getPropertiesForGID(i);
		if (propMap.getType() == Value::Type::MAP &&
			propMap.asValueMap()["Collectible"].asBool()) {
			return i;
		}
	}
	return -1;
}
