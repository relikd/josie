#include "MapController.h"

#include "Level.h"
#include <sstream>

using namespace cocos2d;

TMXLayer *_tilemapBackground;
TMXLayer *_metaLayer;

MapController::MapController() {
	_tilemapBackground = NULL;
	_metaLayer = NULL;
	map = NULL;
	level = NULL;
}
MapController::~MapController() {}

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
}


//
// Tilemap collision etc.
//

bool MapController::hasCollisionBetweenPoints(cocos2d::Point a, cocos2d::Point b)
{
	Point mpa = this->getTileAt(a);
	Point mpb = this->getTileAt(b);

	int direction = (mpa.x<=mpb.x) ? 1 : -1;

	// horizontal
	for (int i=0; i<20; i++) { // prevent infinite loop
		TilePropertyType tp = this->getTilePropertyFromGID(mpa);
		CCLOG("COLLISION-X: %d (%1.3f)", tp,mpa.x);
		if (tp == TilePropertyCollision) return true;
		if (mpa.x == mpb.x) break;
		mpa.x += direction;
	}
	// vertical
	for (int i=0; i<20; i++) { // prevent infinite loop
		TilePropertyType tp = this->getTilePropertyFromGID(mpa);
		CCLOG("COLLISION-Y: %d (%1.3f)", tp,mpa.y);
		if (tp == TilePropertyCollision) return true;
		if (mpa.y == mpb.y) break;
		mpa.y += direction;
	}

	return false;
}

//wandelt position in Tilemap Koordinate um
Point MapController::getTileAt(Point position) {
	int x = position.x  / map->getTileSize().width;
	int y = ((map->getMapSize().height * map->getTileSize().height)
			- position.y) / map->getTileSize().height;
	return Point(x, y);
}

TilePropertyType MapController::getTilePropertyFromGID(Point gid) {
	int tileGID = _metaLayer->getTileGIDAt(gid);
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
