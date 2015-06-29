#include "MapController.h"
#include "CollisionLayer.h"
#include "StageHazard.h"

using namespace cocos2d;
using namespace std;
typedef struct TilePointOffset { int x; int y; float offsetX; float offsetY; }_TilePointOffset;


MapController::MapController() {
	_collisionMap = new long[1];
	_deathlyMap = new long[1];
	_maxCoins = 0;
}
MapController::~MapController() {
	delete _collisionMap;
	delete _deathlyMap;
}

MapController* MapController::initWithLevel(int level, int sub_level)
{
	std::ostringstream mapstr;
	mapstr << "tilemaps/" << level << "." << sub_level << ".tmx";

	MapController* tmc = new MapController();
	if (tmc->initWithTMXFile(mapstr.str()))
	{
		tmc->autorelease();
		tmc->initOptions();
	}

	return tmc;
}

void MapController::initOptions()
{
	getLayer("Meta_layer")->setVisible(true);

	for (const auto& child : getChildren()) {
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}

	this->reinitializeMap();
}

//
// :::::::::: Gameplay Functionality ::::::::::
//

void MapController::setPlayerAsListener(CollisionLayer *player)
{
	for(CollisionLayer* coll : _interactables)
	{
		coll->setCollisionListener(player);
	}
}

bool MapController::checkDeathly(Rect bounds) {
	TilePointOffset pos = this->getTilePointOffset(Vec2(bounds.origin.x, bounds.getMaxY()));
	int playerWidthInTiles = ceil((pos.offsetX + bounds.size.width) / _tileSize.width);
	int playerHeightInTiles = ceil((pos.offsetY + bounds.size.height) / _tileSize.height);

	for (int i = 0; i < playerWidthInTiles; i++) {
		long col = _deathlyMap[pos.x + i];
		int numTiles = playerHeightInTiles;
		col >>= pos.y;
		while (numTiles--) {
			if (col&1)
				return true;
			col >>= 1;
		}
	}
	return false;
}

float MapController::getLevelProgress(cocos2d::Rect bounds)
{
	return (bounds.getMaxX() / ((_mapSize.width-30) * _tileSize.width))*100;
}

void MapController::collectCoin(CollisionLayer *coin)
{
	coin->removeFromParent();
	this->_interactables.eraseObject(coin);
}


//
// :::::::::: Collision Detection ::::::::::
//

float MapController::collisionDiffTop(Rect bounds)
{
	TilePointOffset pos = this->getTilePointOffset(Vec2(bounds.origin.x, bounds.getMaxY()));
	int numTiles = ceil((pos.offsetX + bounds.size.width) / _tileSize.width);
	float topDistance = 9999;
	for (int i=0; i<numTiles; i++)
	{
		long col = _collisionMap[pos.x+i];
		int distance = 9999;
		for (int u=0; u<pos.y; u++)
		{
			if (col&1) distance = 0;
			else distance += _tileSize.height;
			col>>=1;
		}
		if (col&1) // already in collision
			return pos.offsetY -0.1f - _tileSize.height;
		if (distance < topDistance) topDistance = distance;
	}
	return topDistance + pos.offsetY -0.1f;
}

float MapController::collisionDiffBottom(Rect bounds)
{
	TilePointOffset pos = this->getTilePointOffset(bounds.origin);
	int numTiles = ceil((pos.offsetX + bounds.size.width) / _tileSize.width);
	float bottomDistance = 9999;
	for (int i=0; i<numTiles; i++)
	{
		long col = _collisionMap[pos.x+i];
		int distance = 0;
		col >>= pos.y;
		if (col==0) continue; // infinite distance. something like a hole
		while (!(col&1)) {
			distance += _tileSize.height;
			col>>=1;
		}
		if (distance < bottomDistance) bottomDistance = distance;
	}
	return bottomDistance - pos.offsetY -0.1f;
}

float MapController::collisionDiffRight(Rect bounds)
{
	TilePointOffset pos = this->getTilePointOffset(Vec2(bounds.getMaxX(), bounds.getMaxY()));
	int playerHeightInTiles = ceil((pos.offsetY+bounds.size.height) / _tileSize.height);

	for (int i=0; i<2; i++) // get current and next column
	{
		int numTiles = playerHeightInTiles;
		long col = _collisionMap[pos.x+i];
		col >>= pos.y;
		while (numTiles--) {
			if (col&1) return i*_tileSize.width - pos.offsetX -0.1f;
			col>>=1;
		}
	}
	return 2*_tileSize.width - pos.offsetX; // collision at least one tile away
}


//
// :::::::::: Other Functionality ::::::::::
//

void MapController::reinitializeMap(bool re_collision, bool re_interactables)
{
	if (re_collision) {
		this->initCollisionMap();
		this->initDeathlyArray();
	}
	if (re_interactables) {
		this->initInteractables();
	}
}

void MapController::initInteractables()
{
	for (CollisionLayer* coll : _interactables) {
		coll->removeFromParent();
	}
	_interactables.clear();
	_maxCoins = 0;

	int collectGID = this->getGIDForName("Collectible");
	int hazardGID = this->getGIDForName("Hazard");
	TMXLayer *meta = getLayer("Meta_layer");

	for (int x=0; x<(int)_mapSize.width; x++)
	{
		for (int y=0; y<(int)_mapSize.height; y++)
		{
			int testGID = meta->getTileGIDAt(Vec2(x,y));
			if (testGID == hazardGID)
			{
				StageHazard *hazard = StageHazard::createAt(coordinateFromTilePoint(Vec2(x,y)));
				hazard->setLocalZOrder(6);
				this->addChild(hazard);
				_interactables.pushBack(hazard);
			}
			else if (testGID == collectGID)
			{
				CollisionLayer *coin = CollisionLayer::createCoinSprite();
				coin->setPosition(coordinateFromTilePoint(Vec2(x,y)));
				coin->setLocalZOrder(6);
				this->addChild(coin);
				_interactables.pushBack(coin);
				_maxCoins++;
			}
		}
	}
}

//For DeadlyThorn Collision
void MapController::initDeathlyArray()
{
	int mapWidth = (int)_mapSize.width;
	int collisionGID = this->getGIDForName("Deadly");

	delete _deathlyMap;
	_deathlyMap = new long[mapWidth+10]; // Free the Space !

	for (int x=0; x < mapWidth+10; x++) {
		_deathlyMap[x] = 0; // just a few extra columns for collision
		if (x < mapWidth) {
			long column = this->getColumnBitmapForGID(x, collisionGID);
			_deathlyMap[x] = column;
		}
	}
}

void MapController::initCollisionMap()
{
	int mapWidth = (int)_mapSize.width;
	int collisionGID = this->getGIDForName("Collision");

	delete _collisionMap;
	_collisionMap = new long[mapWidth+10]; // remember to free space

	for (int x=0; x < mapWidth+10; x++) {
		_collisionMap[x] = 0; // just a few extra columns for collision
		if (x < mapWidth) {
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
	int x = (position.x + mapOffsetX) / _tileSize.width;
	int y = ((_mapSize.height * _tileSize.height) - position.y) / _tileSize.height;
	return Point(x, y);
}*/

Point MapController::coordinateFromTilePoint(Point tileCoord) {
	Point pos;
	pos.x = tileCoord.x * _tileSize.width;
	pos.y = (_mapSize.height -1 -tileCoord.y) * _tileSize.height;
	pos += _tileSize/2; // midpoint
	return pos;
}

TilePointOffset MapController::getTilePointOffset(Point point)
{
	TilePointOffset tpo;
	float y = ((_mapSize.height * _tileSize.height) - point.y);
	tpo.offsetX = fmod(point.x, _tileSize.width); // + mapOffsetX
	tpo.offsetY = fmod(y, _tileSize.height);
	tpo.x = (point.x) / _tileSize.width; // + mapOffsetX
	tpo.y = (y / _tileSize.height);
	return tpo;
}

long MapController::getColumnBitmapForGID(int x, int tile_gid)
{
	TMXLayer *meta = getLayer("Meta_layer");
	long col=0;
	for (int i=_mapSize.height; i>0; i--) {
		col<<=1;
		int gid = meta->getTileGIDAt(Vec2(x,i-1));
		col |= (gid==tile_gid);
	}
	return col;
}

int MapController::getGIDForName(const std::string& name)
{
	for (auto x : _tileProperties) {
		Value propMap = x.second;
		if (propMap.getType() == Value::Type::MAP &&
			propMap.asValueMap()[name].asBool()) {
			return (int)(x.first);
		}
	}
	return -1;
}
