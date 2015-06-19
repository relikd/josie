#include "MapController.h"
#include "CollisionLayer.h"

using namespace cocos2d;
using namespace std;
typedef struct TilePointOffset { int x; int y; float offsetX; float offsetY; }_TilePointOffset;


MapController::MapController() {
	_collisionMap = new long[1];
}
MapController::~MapController() {
	delete _collisionMap;
	CCLOG("~MapController");
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

bool MapController::tryCollect(CollisionLayer *player)
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

float MapController::getLevelProgress(cocos2d::Rect bounds)
{
	return (bounds.getMaxX() / (_mapSize.width * _tileSize.width))*100;
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

void MapController::reinitializeMap(bool re_collision, bool re_coins)
{
	if (re_collision) {
		this->initCollisionMap();

	}
	if (re_coins) {
		for (CollisionLayer* coin : this->_coins) {
			coin->removeFromParent();
		}
		_coins.clear();
		this->initCollectableArray();
		this->initDeathlyArray();
	}
}

void MapController::initCollectableArray()
{
	int collectGID = this->getGIDForName("Collectible");

	for (int x=0; x<(int)_mapSize.width; x++)
	{
		for (int y=0; y<(int)_mapSize.height; y++)
		{
			if (collectGID == getLayer("Meta_layer")->getTileGIDAt(Vec2(x,y)))
			{
				CollisionLayer *coin = CollisionLayer::createCoinSprite();
				coin->setPosition(coordinateFromTilePoint(Vec2(x,y-1))); // 1 tile above collectable
				_coins.pushBack(coin);
				this->addChild(coin);
			}
		}
	}
}
//For DeadlyThorn Colission
void MapController::initDeathlyArray()
{
	//Get Possible Collisions
	/*std::string possColl[] = {"Thorn", "ThornUp", "ThornUp2", "PflockEcke", "Pflock"};
	int collectGID[];
	for (int x=0; x<=4;x++){
	collectGID[x] = this->getGIDForName(possColl[x]);
	}

	//Create Deadly Collision Sprites
	for (int x=0; x<(int)_mapSize.width; x++)
	{
		for (int y=0; y<(int)_mapSize.height; y++)
		{
			if (collectGID[0] == getLayer("Meta_layer")->getTileGIDAt(Vec2(x,y)))
			{

				CollisionLayer* thornColl = CollisionLayer::createWithSize(72.f,72.f);
				thornColl.insertImageName("Collisions/Dorne00");
				thornColl->setPosition(coordinateFromTilePoint(Vec2(x,y)));
				_deathies.pushBack(thornColl);
				this->addChild(thornColl);

			}
			else if (collectGID[1] == getLayer("Meta_layer")->getTileGIDAt(Vec2(x,y)))
			{
				CollisionLayer* thornColl = CollisionLayer::createWithSize(72.f,72.f);
				thornColl.insertImageName("Collisions/Dorne01");
				thornColl->setPosition(coordinateFromTilePoint(Vec2(x,y)));
				_deathies.pushBack(thornColl);
				this->addChild(thornColl);
			}
			else if (collectGID[2] == getLayer("Meta_layer")->getTileGIDAt(Vec2(x,y)))
			{
				CollisionLayer* thornColl = CollisionLayer::createWithSize(72.f,72.f);
				thornColl.insertImageName("Collisions/Dorne02");
				thornColl->setPosition(coordinateFromTilePoint(Vec2(x,y)));
				_deathies.pushBack(thornColl);
				this->addChild(thornColl);
			}
			else if (collectGID[2] == getLayer("Meta_layer")->getTileGIDAt(Vec2(x,y)))
			{
				CollisionLayer* thornColl = CollisionLayer::createWithSize(72.f,72.f);
				thornColl.insertImageName("Collisions/Pflock00");
				thornColl->setPosition(coordinateFromTilePoint(Vec2(x,y)));
				_deathies.pushBack(thornColl);
				this->addChild(thornColl);
			}
			else {
				CollisionLayer* thornColl = CollisionLayer::createWithSize(72.f,72.f);
				thornColl.insertImageName("Collisions/Pflock01");
				thornColl->setPosition(coordinateFromTilePoint(Vec2(x,y)));
				_deathies.pushBack(thornColl);
				this->addChild(thornColl);
			}

		}
	}*/
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
	long col=0;
	for (int i=_mapSize.height; i>0; i--) {
		col<<=1;
		int gid = getLayer("Meta_layer")->getTileGIDAt(Vec2(x,i-1));
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
