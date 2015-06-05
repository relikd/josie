#include "MapController.h"

#include "Level.h"
#include <sstream> // string formatter ostringstream
#include <ctgmath> // fmod()

using namespace cocos2d;

typedef struct TilePointOffset { int x; int y; float offsetX; float offsetY; }_TilePointOffset;


MapController::MapController() {
	_metaLayer = NULL;
	map = NULL;
	mapOffsetX = 0.0f;

	_collisionMap = new long[1];
}
MapController::~MapController() {
	delete _collisionMap;
	CCLOG("~MapController");
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
//initialisiert MAp nicht aus Datei direkt sondern aus gegebenem TMXTiledMap Objekt
MapController* MapController::initWithObject(TMXTiledMap* map)
{
	MapController* tmc = new MapController();

	tmc->map = map;
	tmc->initOptions();

	return tmc;
}

void MapController::initOptions()
{
	//_tilemapBackground = map->getLayer("Background_layer");
	_metaLayer = map->getLayer("Meta_layer");
	_metaLayer->setVisible(true);

	for (const auto& child : map->getChildren()) {
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
	this->initCollisionMap();
	this->initCollectableArray();
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

bool MapController::tryCollect(Rect playerBounds)
{
	Size s = map->getTileSize();

	for(int i=0; i<200; i++)
	{
		IntPoint p = _coinArray[i];
		if (p.x==0 && p.y==0) break; // never place a coin at 0,0

		Rect n;
		n.origin.x = p.x*s.width - mapOffsetX;
		n.origin.y = ((map->getMapSize().height-1) * map->getTileSize().height) - p.y*s.height;
		n.size = s;

		if (playerBounds.intersectsRect(n)) {
			_coinArray[i] = {0,1};
			collectAt(Vec2(p.x,p.y));
		}
	}
	return false;
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

void MapController::collectAt(Point tileCoord) {
	_metaLayer->removeTileAt(tileCoord);
	map->getLayer("Foreground_layer")->removeTileAt(tileCoord);
}

TilePointOffset MapController::getTilePointOffset(Point point)
{
	TilePointOffset tpo;
	float y = ((map->getMapSize().height * map->getTileSize().height) - point.y);
	tpo.offsetX = fmod(point.x + mapOffsetX, map->getTileSize().width);
	tpo.offsetY = fmod(y, map->getTileSize().height);
	tpo.x = (point.x + mapOffsetX) / map->getTileSize().width;
	tpo.y = (y / map->getTileSize().height);
	return tpo;
}

void MapController::initCollectableArray()
{
	int mapWidth = (int)map->getMapSize().width;
	int mapHeight = (int)map->getMapSize().height;
	int collectGID = getGIDForCollectable();
	int counter = 0;

	for (int x=0; x<mapWidth; x++)
	{
		for (int y=0; y<mapHeight; y++)
		{
			if (collectGID == _metaLayer->getTileGIDAt(Vec2(x,y))) {
				_coinArray[counter++] = {x,y};
				if (counter==200) return; // can save only 200 coins
			}
		}
	}
}

void MapController::initCollisionMap()
{
	int mapWidth = (int)map->getMapSize().width;
	int collisionGID = this->getGIDForCollision();

	delete _collisionMap;
	_collisionMap = new long[mapWidth+6]; // remember to free space

	for (int x=0; x<mapWidth+6; x++) {
		_collisionMap[x] = 0; // just a few extra columns for collision
		if (x<mapWidth) {
			long column = this->getColumnBitmapForGID(x, collisionGID);
			_collisionMap[x] = column;
		}
	}
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
