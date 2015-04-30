#include "MapController.h"

#include "Level.h"
#include <sstream> // string formatter ostringstream
#include <ctgmath> // fmod()

using namespace cocos2d;

typedef struct TilePointOffset { int x; int y; float offsetX; float offsetY; }_TilePointOffset;

// private VARS
long *_collisionMap;

TMXLayer *_tilemapBackground;
TMXLayer *_metaLayer;


MapController::MapController() {
	_tilemapBackground = NULL;
	_metaLayer = NULL;
	map = NULL;

	_collisionMap = new long[1];
}
MapController::~MapController() {
	delete _collisionMap;
	CCLOG("MapController destroyed");
}

MapController* MapController::initWithLevel(Level *lvl)
{
	MapController* tmc = new MapController();

	std::ostringstream mapstr;
	mapstr << "tilemaps/" << lvl->currentLevel << "." << lvl->currentSubLevel << ".tmx";
	tmc->map = TMXTiledMap::create(mapstr.str());
	tmc->initOptions();

	return tmc;
}

void MapController::initOptions()
{
	_tilemapBackground = map->getLayer("Background_layer");
	_metaLayer = map->getLayer("Meta_layer");
	_metaLayer->setVisible(true);

	for (const auto& child : map->getChildren()) {
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
	this->initCollisionMap();
}


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
			col>>=1;
			distance += map->getTileSize().height;
		}
		if (distance < bottomDistance) bottomDistance = distance;
	}
	return bottomDistance - pos.offsetY -0.1f;
}

float MapController::collisionDiffRight(Rect bounds)
{
	TilePointOffset pos = this->getTilePointOffset(Vec2(bounds.getMaxX(), bounds.getMaxY()));
	int numTiles = ceil((pos.offsetY+bounds.size.height) / map->getTileSize().height);

	long col = _collisionMap[pos.x+1];
	col >>= pos.y;
	while (numTiles--) {
		if (col&1) return map->getTileSize().width - pos.offsetX -0.1f;
		col>>=1;
	}
	return 2*map->getTileSize().width - pos.offsetX; // collision at least one tile away
}



//
// :::::::::: Other Functionality ::::::::::
//

TilePropertyType MapController::getTileProperty(Point position) {
	Point tileCoord = getTileAt(position);
	int tileGID = _metaLayer->getTileGIDAt(tileCoord);
	Value propMap = map->getPropertiesForGID(tileGID);

	if (propMap.getType() == Value::Type::MAP) {
		if (propMap.asValueMap()["Collectible"].asBool()) {
			return TilePropertyCollectable;
		} else if (propMap.asValueMap()["Collision"].asBool()) {
			return TilePropertyCollision;
		}
	}
	return TilePropertyNone;
}

//entfernt an uebergebener Position das Tile aus dem Foreground_layer und das dazugehoerige Tile aus dem Meta_layer
void MapController::collectAt(Point position) {
	Point TileCoord = getTileAt(position);
	if (getTileProperty(TileCoord) == TilePropertyCollectable) {
		_metaLayer->removeTileAt(TileCoord); //verhindert, dass Collision Tiles entfernt werden k�nnen;
		TMXLayer* Foreground = map->getLayer("Foreground_layer");
		Foreground->removeTileAt(TileCoord);
	}
}



//
// :::::::::: Internal/Private Functions ::::::::::
//

// wandelt Position in Tilemap Koordinate um
Point MapController::getTileAt(Point position) {
	int x = position.x / map->getTileSize().width;
	int y = ((map->getMapSize().height * map->getTileSize().height)
			- position.y) / map->getTileSize().height;
	return Point(x, y);
}

TilePointOffset MapController::getTilePointOffset(Point point)
{
	TilePointOffset tpo;
	float y = ((map->getMapSize().height * map->getTileSize().height) - point.y);
	tpo.offsetX = fmod(point.x, map->getTileSize().width);
	tpo.offsetY = fmod(y, map->getTileSize().height);
	tpo.x = point.x / map->getTileSize().width;
	tpo.y = (y / map->getTileSize().height);
	return tpo;
}

void MapController::initCollisionMap()
{
	int mapWidth = (int)map->getMapSize().width;
	int collisionGID = this->getGIDForCollision();

	delete _collisionMap;
	_collisionMap = new long[mapWidth]; // remember to free space

	for (int x=0; x<mapWidth; x++) {
		long column = this->getColumnBitmapForGID(x, collisionGID);
		_collisionMap[x] = column;
	}
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

long MapController::getColumnBitmapForGID(int x, int tile_gid)
{
	long col=0;
	for (int i=map->getMapSize().height; i>0; i--) {
		col<<=1;
		int gid = _metaLayer->getTileGIDAt(Vec2(x,i-1));
		col |= (gid==tile_gid);
	}
	return col;
}


