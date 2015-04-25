#include "Level.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "PlayerBoss.h"
#include "PlayerControl.h"
#include <sstream>
#include "AudioUnit.h"

using namespace cocos2d;

Size visibleSize;
Vec2 origin;

Level::Level() {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	player = NULL;
	playerControl = NULL;
	audioUnit = NULL;

	_tileMap = NULL;
	_tilemapBackground = NULL;
	_metaLayer = NULL;

	currentLevel = -1;
	currentSubLevel = -1;
}
Level::~Level() {
}

Level* Level::initWithLevel(int level, int sublevel)
{
	Level *l = new Level();
	l->autorelease();

	l->currentLevel = level;
	l->currentSubLevel = sublevel;

	l->addBackground();
	l->addTilemap();
	l->addPauseButton();

	l->addAudio();
	l->addPlayer();
	l->addPlayerControl();

	// return the scene
	return l;
}

//Method Called by Pausebutton -> "goes back" to MainMenu
void Level::pause(Ref* pSender) {
	audioUnit->stopBackground();
	delete audioUnit;
	Director::getInstance()->popScene();
}

bool Level::isBossLevel()
{
	return (currentSubLevel == 0);
}



//
// ADD to Scene
//

void Level::addTilemap() { //Add TileMap
//lade Tilemap aus Resource Ordner in Abhaenggkeit der uebergebenen level-Nummer
	std::ostringstream tilemap;
	tilemap << "tilemaps/" << currentLevel << "." << currentSubLevel << ".tmx";
	_tileMap = TMXTiledMap::create(tilemap.str());
	_tilemapBackground = _tileMap->getLayer("Background_layer");
	_metaLayer = _tileMap->getLayer("Meta_layer");
	_metaLayer->setVisible(false);
	this->addChild(_tileMap, 0);
	for (const auto& child : _tileMap->getChildren()) {
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
}

void Level::addBackground() {
	//index to string for background loading
	std::ostringstream s;
	s << "backgrounds/bg_" << currentLevel << "." << currentSubLevel << ".png";

	//Add Background Image
	auto background = Sprite::create(s.str());
	background->setPosition(
			Vec2(visibleSize.width / 2 + origin.x,
					visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);
}

void Level::addAudio() {
	audioUnit = AudioUnit::initWithLevel(this);
	audioUnit->playBackground();
}

void Level::addPlayer() {
	//Add Player
	if (this->isBossLevel()) {
		player = (Player*)PlayerBoss::initWithLevel(this);
	} else {
		player = Player::initWithLevel(this);
	}

	player->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + 288));
	this->addChild(player, 1);
}

void Level::addPlayerControl() {
	//add control to scene
	playerControl = PlayerControl::initWithLevel(this);
	this->addChild(playerControl, -1);
}

void Level::addPauseButton() {
	//Add Pause Button in upper right corner
	auto pause = MenuItemImage::create("buttons/pausebutton.png",
			"buttons/pausebutton.png", CC_CALLBACK_1(Level::pause, this));
	pause->setPosition(
			origin.x + visibleSize.width - pause->getContentSize().width,
			origin.y + visibleSize.height - pause->getContentSize().height);
	auto menu = Menu::create(pause, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

}



//
// Tilemap collision etc.
//

//wandelt position in Tilemap Koordinate um
Point Level::getTileAt(Point position) {
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height)
			- position.y) / _tileMap->getTileSize().height;
	return Point(x, y);
}

TilePropertyType Level::getTileProperty(Point position) {
	Point tileCoord = getTileAt(position);
	int tileGID = _metaLayer->getTileGIDAt(tileCoord);
	Value propMap = _tileMap->getPropertiesForGID(tileGID);

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
void Level::collectAt(Point position) {
	Point TileCoord = getTileAt(position);
	if (getTileProperty(TileCoord) == TilePropertyCollectable) {
		_metaLayer->removeTileAt(TileCoord); //verhindert, dass Collision Tiles entfernt werden k�nnen;
		TMXLayer* Foreground = _tileMap->getLayer("Foreground_layer");
		Foreground->removeTileAt(TileCoord);
	}
}
