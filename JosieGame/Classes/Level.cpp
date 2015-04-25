#include "Level.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "PlayerControl.h"
#include <sstream>
#include "AudioUnit.h"

using namespace cocos2d;

int _res_index;
int _res_index_sub;

Size visibleSize;
Vec2 origin;


Level::Level()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	player=NULL;
	playerControl=NULL;
	audioUnit=NULL;

	_tileMap = NULL;
	_tilemapBackground = NULL;
	_metaLayer = NULL;
}
Level::~Level(){}


Scene* Level::createScene(int level, int sublevel)
{

	_res_index = level;
	_res_index_sub = sublevel;

	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Level::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Level::init()
{
	if (!Layer::init())
	{
		return false;
	}

	addBackground();
	
	addTilemap();

	addPauseButton();
	addAudio();
    addPlayer();
    addPlayerControl();
	return true;

}


//Method Called by Pausebutton -> "goes back" to MainMenu
void Level::pause(Ref* pSender)
{
	audioUnit->stopBackground();
	delete audioUnit;
	Director::getInstance()->popScene();
}

void Level::addTilemap()
{//Add TileMap
	//lade Tilemap aus Resource Ordner in Abhaenggkeit der uebergebenen level-Nummer
	std::ostringstream tilemap;
	tilemap << "tilemaps/" << _res_index << "." << _res_index_sub << ".tmx";
	_tileMap = TMXTiledMap::create(tilemap.str());
	_tilemapBackground = _tileMap->getLayer("Background_layer");
	_metaLayer = _tileMap->getLayer("Meta_layer");
	_metaLayer->setVisible(false);
	this->addChild(_tileMap, 0);
	for (const auto& child : _tileMap->getChildren()) {
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}
}

void Level::addBackground()
{
	//index to string for background loading
	std::ostringstream s;
	s << "backgrounds/bg_"<< _res_index <<"."<<_res_index_sub << ".png";

	//Add Background Image
		auto background = Sprite::create(s.str());
		background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(background, 0);
}

void Level::addAudio()
{
	audioUnit = AudioUnit::initWithLevel(1,1);
	audioUnit->playBackground();
}

void Level::addPlayer()
{
	//Add Player
	player = Player::initWithLevel(this);
	player->setPosition(Vec2(origin.x + visibleSize.width / 5, origin.y + 288));
	this->addChild(player, 1);
	player->scheduleUpdate();

}

void Level::addPlayerControl()
{
	//add control to scene
    playerControl = PlayerControl::initWithLevel(this);
    this->addChild(playerControl,-1);
}

void Level::addPauseButton()
{
	//Add Pause Button in upper right corner
	auto pause = MenuItemImage::create("buttons/pausebutton.png",
										"buttons/pausebutton.png",
										CC_CALLBACK_1(Level::pause,this));
	pause->setPosition(origin.x + visibleSize.width - pause->getContentSize().width,
						origin.y + visibleSize.height -pause->getContentSize().height);
	auto menu = Menu::create(pause, NULL);
			    menu->setPosition(Vec2::ZERO);
			    this->addChild(menu, 1);

}

//wandelt position in Tilemap Koordinate um
Point Level::getTileAt(Point position) {
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height)
			- position.y) / _tileMap->getTileSize().height;
	return Point(x, y);
}

TilePropertyType Level::getTileProperty(Point position)
{
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

//entfernt an �bergebener Position das Tile aus dem Foreground_layer und das dazugeh�rige Tile aus dem Meta_layer
void Level::collectAt(Point position)
{
	Point TileCoord = getTileAt(position);
	TMXLayer* Foreground = _tileMap->getLayer("Foreground_layer");
	if (getTileProperty(TileCoord) == TilePropertyCollectable)
		_metaLayer->removeTileAt(TileCoord);//verhindert, dass Collision Tiles entfernt werden k�nnen;
	Foreground->removeTileAt(TileCoord);
}
