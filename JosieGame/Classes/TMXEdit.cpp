/*
 * TMXEdit.cpp
 *
 *  Created on: 30.05.2015
 *      Author: Jonas
 */
#include "TMXEdit.h"
#include "MapController.h"

using namespace cocos2d;



//GIDs
const int TOPDIRT[] = { 1, 2, 3, 4 };
const int DIRT[] = { 5, 6, 7, 8 };
const int COIN = 19;
const int COLLIDE = 20;
const int KILL = 21;
const int HAZARD = 22;
const uint32_t THORN[] = { 24, 24 | kTMXTileHorizontalFlag, 25 | kTMXTileDiagonalFlag };
const int STAKE = 29;
const int FLOATSTART = 9;
const int FLOATEND = 12;
const int FLOATING[] = { 10, 11, 13, 14 };
const int WATERSTART = 15;
const int WATER[] = {16, 17};
const int WATEREND = 18;

TMXEdit::TMXEdit() {
	// TODO Auto-generated constructor stub
	map = MapController::initWithLevel(0, 1);
	_backgroundLayer = NULL;
	_metaLayer = NULL;
	_foregroundLayer = NULL;
	_partlength = 6;
	_minheight = 14;
	_maxheight = 9;
	//Higher Values mean lessHazards/Coins
	_coinheight = 2;
	_hazardDistanceMin = 15;
	_coins = 10;
	_minJumpDistance = 2;
	//Highest distance that a jump can require in addition to _minJumpDistance (max for both together is 7)
	_maxJumpDistance = 5;
}

TMXEdit::~TMXEdit() {
	// TODO Auto-generated destructor stub
}
//Schwierigkeit bisher nur Konsistent für difficulty im Berecih 1-3
MapController* TMXEdit::makeMap(int difficulty) {
	TMXEdit* maker = new TMXEdit();
	//die Formeln und Werte sind spezifisch auf 3 Schwierigkeitsgrade ausgelegta
	maker->_hazardDistanceMin = -8*difficulty+32;
	maker->_coins = 25*pow(difficulty,2) -65*difficulty+50;
	CCLOG("%d _coins coins",maker->_coins);
	maker->_minJumpDistance = difficulty +2;
	maker->_maxJumpDistance = 2;
	maker->getLayers();
	maker->fillLevel();
	maker->map->reinitializeMap();
	return maker->map;


}

void TMXEdit::getLayers() {
	_backgroundLayer = map->getLayer("Background_layer");
	_foregroundLayer = map->getLayer("Foreground_layer");
	_metaLayer = map->getLayer("Meta_layer");
}

void TMXEdit::fillLevel() {
	int width = map->getMapSize().width;
	int x = 20;
	int height = 13;
	while (x < width - (46 + 2 * _partlength)) {

		switch (arc4random() % 12) {
		case 0:
			x = makePillars(x, height);
			x = placeGroundLength(x, height, 2 + arc4random() % 5);
			break;

		case 1:

			x = makeJumpAndSlide(x, height);
			x = placeGroundLength(x, height, 2 + arc4random() % 5);
			break;
		case 2:
			x = makeThornField(x, height);
			break;

		case 3:
			x = makeFloating(x, height);
			x = placeGroundLength(x, height, 2 + arc4random() % 5);
			break;

		case 4:
			x = makeSlide(x, height);
			break;

		case 5:
			x = makeSpikeSlide(x, height);
			break;
		case 6:
			x= makeWater(x,height);
			break;
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:

			height = newHeight();
			x = placeGroundLength(x, height, 2 + arc4random() % 4);

			break;
		}
	}
	while(x < width - 39){
		placeGround(x, height);
		x++;
	}
	placeHazards(_hazardDistanceMin);
	placeCoins(_coins);
	}

void TMXEdit::placeGround(int x, int y) {

	_backgroundLayer->setTileGID(TOPDIRT[arc4random() % 4], Vec2(x, y));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, y));
	if (y < _minheight)
		placeDirt(x, y + 1);
}

void TMXEdit::placeDirt(int x, int y) {
	_backgroundLayer->setTileGID(DIRT[arc4random() % 4], Vec2(x, y));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, y));
	if (y < _minheight)
		placeDirt(x, y + 1);
}

int TMXEdit::makePillars(int x, int height) {
	height -= 3;
	x += 2;
	int done = x + _partlength * (1 + arc4random() % 2);
	while (x <= done) {
		placeGround(x++, height);
		placeGround(x++, height);

		x += _minJumpDistance + (arc4random() % _maxJumpDistance);
	}

	return x;
}

int TMXEdit::makeThornField(int x, int height) {
	int done = x + _partlength * (1 + arc4random() % 2);
	x = placeGroundLength(x, height, (2 + arc4random() % 2));
	while (x < done) {
		x = placeGroundLength(x, height,1+ arc4random() % 3);
		uint32_t thorn = THORN[arc4random() % 3];
		TMXTileFlags flags = (TMXTileFlags)(thorn & kTMXFlipedAll);
		_backgroundLayer->setTileGID(thorn&kTMXFlippedMask, Vec2(x, height - 1), flags);
		_metaLayer->setTileGID(KILL, Vec2(x, height - 1));
		x = placeGroundLength(x, height, 2 + arc4random() % 3);
	}

	return x;
}

int TMXEdit::makeFloating(int x, int height) {
	x += _minJumpDistance + arc4random() % _maxJumpDistance-1;
	int done = x + _partlength * (1 + arc4random() % 2);
	while (x <= done) {
		x = FloatingPlatform(x, height, 3+arc4random()%3);
		x += _minJumpDistance + arc4random() % _maxJumpDistance;
	}

	return x;

}

int TMXEdit::makeSlide(int x, int height) {
	x = placeGroundLength(x, height, 2);

	_backgroundLayer->setTileGID(STAKE + 0x20000000, Vec2(x, height - 2));
	_metaLayer->setTileGID(KILL, Vec2(x, height - 2));
	placeGround(x, height);
	int length = 1 + arc4random() % 2;
	while (length-- > 0) {
		_backgroundLayer->setTileGID(FLOATING[arc4random() % 4],
				Vec2(++x, height - 2));
		if (arc4random() % 2 == 0) {
			_backgroundLayer->setTileGID(STAKE, Vec2(x, height - 3));

			_metaLayer->setTileGID(KILL, Vec2(x, height - 3));
		}
		_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 2));
		placeGround(x, height);
	}
	_backgroundLayer->setTileGID(FLOATEND, Vec2(++x, height - 2));

	if (arc4random() % 2 == 0) {
		_backgroundLayer->setTileGID(STAKE, Vec2(x, height - 3));

		_metaLayer->setTileGID(KILL, Vec2(x, height - 3));
	}
	_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 2));
	x = placeGroundLength(x, height, 2);
	return x;
}

int TMXEdit::makeSpikeSlide(int x, int height) {
	x = placeGroundLength(x, height, 2);

	_backgroundLayer->setTileGID(STAKE + 0x20000000, Vec2(x, height - 3));
	_metaLayer->setTileGID(KILL, Vec2(x, height - 3));

	placeGround(x, height);
	int length = 2 + arc4random() % 2;
	while (length-- > 0) {
		_backgroundLayer->setTileGID(FLOATING[arc4random() % 4],
				Vec2(++x, height - 3));
		_backgroundLayer->setTileGID(STAKE + 0x40000000, Vec2(x, height - 2));
		_metaLayer->setTileGID(KILL, Vec2(x, height - 2));

		_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 3));
		placeGround(x, height);
	}
	_backgroundLayer->setTileGID(FLOATEND, Vec2(++x, height - 3));
	_backgroundLayer->setTileGID(STAKE + 0x40000000, Vec2(x, height - 2));
	_metaLayer->setTileGID(KILL, Vec2(x, height - 2));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 3));
	x = placeGroundLength(x, height, 2);
	return x;

}

int TMXEdit::makeJumpAndSlide(int x, int height) {
	x += 2;
	int dif = 4 + arc4random() % 2;
	int length = 5 + arc4random() % 3;
	FloatingPlatform(x+2, height-dif, length -3);
	x= FloatingPlatform(x,height-2,length);
	return x + _minJumpDistance + arc4random()%_maxJumpDistance;

}

int  TMXEdit::makeWater(int x ,int height){
	x = placeGroundLength(x, height, 2);
	int done = x + _partlength * (1 + arc4random() % 2);
	while (x <= done){
		_backgroundLayer->setTileGID(WATERSTART,Vec2(x,height));
		_metaLayer->setTileGID(KILL,Vec2(x,height));
		if (height<_minheight) placeDirt(x,height+1);
		x++;
		int repeat = _minJumpDistance + x + arc4random()%_maxJumpDistance -2;
		do{
			_backgroundLayer->setTileGID(WATER[arc4random()%2],Vec2(x,height));
			_metaLayer->setTileGID(KILL,Vec2(x,height));
			if (height<_minheight) placeDirt(x,height+1);
			x++;
		}while(x<repeat);
		_backgroundLayer->setTileGID(WATEREND,Vec2(x,height));
		_metaLayer->setTileGID(KILL,Vec2(x,height));
		if (height<_minheight) placeDirt(x,height+1);
		x++;

		x = placeGroundLength(x, height, 2);
	}
	return x;
}

int TMXEdit::FloatingPlatform(int x, int height, int length) {
	if (length < 2) length = 2;
	_backgroundLayer->setTileGID(FLOATSTART, Vec2(x, height - 2));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 2));
	length -= 2;
	while (length-- > 0) {
		_backgroundLayer->setTileGID(FLOATING[arc4random() % 4],
				Vec2(++x, height - 2));
		_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 2));
	}
	_backgroundLayer->setTileGID(FLOATEND, Vec2(++x, height - 2));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 2));
	return x;

}

int TMXEdit::placeGroundLength(int x, int height, int length) {
	int done = x + length;
	while (x <= done)
		placeGround(x++, height);

	return x;
}

void TMXEdit::placeHazards(int distance){
	for (int x = 50; x < map->getMapSize().width-distance *3; x+=distance * (1 + arc4random()%3)){
		if(_metaLayer->getTileGIDAt(Vec2(x,0)) == 0)
			_metaLayer->setTileGID(HAZARD,Vec2(x,0));
	}

}

void TMXEdit::placeCoins(int number) {
	int counter = 0;
	int distance = (int) map->getMapSize().width/number;
	CCLOG("%d!!!!!!!!!!!!!!!!!!!!!!", distance);
	int x = 50;
	while (counter < number) {
		int y = isSafe(x);
		if (y > 0) {
			placeCoin(x, y);
			counter++;
			x = x - (x % distance) + distance;
			if(x >  map->getMapSize().width -50) x= 50;
		} else { //TODO
			x += 1;
		}
	}

}
int TMXEdit::isSafe(int x) {
	int y = getHighestTile(x); //TODO
	if (y <= 0)
		return -1;
	if (_metaLayer->getTileGIDAt(Vec2(x, y-2)) == COIN) return -1;
	int Gid = _metaLayer->getTileGIDAt(Vec2(x, y));
	if (Gid == COLLIDE) {
		int s = checkSurroundings(x, y-_coinheight);
		if (s > 0) {
			return y - _coinheight;
		}
	}
	return -1;
}

int TMXEdit::getHighestTile(int x) {
	int ret = -1;
	for (int y = 14; y >= 0; y--) {
		int Gid = _backgroundLayer->getTileGIDAt(Vec2(x, y));

		if (Gid != 0)
			ret = y;
	}
	return ret;
}

int TMXEdit::checkSurroundings(int x, int y) {
	if (_backgroundLayer->getTileGIDAt(Vec2(x - 1, y)) == 0
			&& _backgroundLayer->getTileGIDAt(Vec2(x + 1, y)) == 0
			&& _backgroundLayer->getTileGIDAt(Vec2(x - 1, y+_coinheight-1)) == 0
			&& _backgroundLayer->getTileGIDAt(Vec2(x - 2, y+_coinheight-1)) == 0
			&& _backgroundLayer->getTileGIDAt(Vec2(x + 1, y+_coinheight-1)) == 0
			&& _backgroundLayer->getTileGIDAt(Vec2(x + 1, y-1)) == 0)
				return 1;
	return -1;
}

void TMXEdit::placeCoin(int x, int height) {
	_metaLayer->setTileGID(COIN, Vec2(x, height));
}
int TMXEdit::newHeight() {
	return _minheight - (arc4random() % (_minheight - _maxheight));
}
