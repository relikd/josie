/*
 * TMXEdit.cpp
 *
 *  Created on: 30.05.2015
 *      Author: Jonas
 */
//TODO: SPIKES auf tödlich setzen wenn collision dafür implementiert ist!
#include "TMXEdit.h"
#include "MapController.h"

using namespace cocos2d;

const int PARTLENGTH = 6;
const int MINHEIGHT = 14;
const int MAXHEIGHT = 8;
const int COINHEIGHT = 2;

//GIDs
const int TOPDIRT[] = { 1, 2, 3, 4 };
const int DIRT[] = { 5, 6, 7, 8 };
const int COLLIDE = 20;
const int COIN = 19;
const int KILL = 21;
const int THORN[] = { 24, 24 + 0x80000000, 25 + 0x20000000 };
const int STAKE = 29;
const int FLOATSTART = 9;
const int FLOATEND = 12;
const int FLOATING[] = { 10, 11, 13, 14 };

TMXEdit::TMXEdit() {
	// TODO Auto-generated constructor stub
	map = MapController::initWithLevel(0, 1);
	_backgroundLayer = NULL;
	_metaLayer = NULL;
	_foregroundLayer = NULL;
}

TMXEdit::~TMXEdit() {
	// TODO Auto-generated destructor stub
}

//TODO: erschaffe map mit parametern
MapController* TMXEdit::makeMap() {
	TMXEdit* maker = new TMXEdit();
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
	while (x < width - (4 * PARTLENGTH)) {

		switch (arc4random() % 10) {
		case 0:
			x = makePillars(x, height);
			x = placeGroundLength(x, height, 2 + arc4random() % 5);
			break;

		case 1:

			x = makeJumpAndSlide(x, height);
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
		case 7:
		case 8:
		case 9:

			height = newHeight();
			x = placeGroundLength(x, height, 2 + arc4random() % 4);

			break;
		}
	}
	while (x < width) {
		placeGround(x, height);
		x++;
	}
	CCLOG("LEVEL CREATED, ATTEMPTING COINS");
	placeCoins(25);
	CCLOG("COINS DONE");
}

void TMXEdit::placeGround(int x, int y) {

	_backgroundLayer->setTileGID(TOPDIRT[arc4random() % 4], Vec2(x, y));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, y));
	if (y < MINHEIGHT)
		placeDirt(x, y + 1);
}

void TMXEdit::placeDirt(int x, int y) {
	_backgroundLayer->setTileGID(DIRT[arc4random() % 4], Vec2(x, y));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, y));
	if (y < MINHEIGHT)
		placeDirt(x, y + 1);
}

int TMXEdit::makePillars(int x, int height) {
	height -= 3;
	x += 2;
	int done = x + PARTLENGTH * (1 + arc4random() % 2);
	while (x <= done) {
		placeGround(x++, height);
		placeGround(x++, height);

		x += 2 + (arc4random() % 5);
	}

	return x;
}

int TMXEdit::makeThornField(int x, int height) {
	int done = x + PARTLENGTH * (1 + arc4random() % 2);
	x = placeGroundLength(x, height, (2 + arc4random() % 2));
	while (x < done) {
		x = placeGroundLength(x, height,1+ arc4random() % 3);
		_backgroundLayer->setTileGID(THORN[arc4random() % 3],
				Vec2(x, height - 1));
		_metaLayer->setTileGID(KILL, Vec2(x, height - 1));
		x = placeGroundLength(x, height, 2 + arc4random() % 3);
	}

	return x;
}

int TMXEdit::makeFloating(int x, int height) {
	x += 2 + arc4random() % 3;
	int done = x + PARTLENGTH * (1 + arc4random() % 2);
	while (x <= done) {
		x = FloatingPlatform(x, height);
		x += 3 + arc4random() % 3;
	}

	return x;

}

int TMXEdit::makeSlide(int x, int height) {
	x = placeGroundLength(x, height, 2);

	_backgroundLayer->setTileGID(STAKE + 0x20000000, Vec2(x, height - 2));
	_metaLayer->setTileGID(KILL, Vec2(x, height - 2));
	placeGround(x, height);
	int length = 2 + arc4random() % 2;
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
	_backgroundLayer->setTileGID(FLOATSTART, Vec2(x, height - 2));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 2));

	_backgroundLayer->setTileGID(FLOATSTART, Vec2(x + 1, height - dif));
	_metaLayer->setTileGID(COLLIDE, Vec2(x + 1, height - dif));

	int length = 3 + arc4random() % 3;
	while (length-- > 0) {

		_backgroundLayer->setTileGID(FLOATING[arc4random() % 4],
				Vec2(++x, height - 2));
		_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 2));
		if (length == 1) {
			_backgroundLayer->setTileGID(FLOATEND, Vec2(x, height - dif));
			_metaLayer->setTileGID(COLLIDE, Vec2(x, height - dif));
			break;
		}
		_backgroundLayer->setTileGID(FLOATING[arc4random() % 4],
				Vec2(x + 1, height - dif));
		_metaLayer->setTileGID(COLLIDE, Vec2(x + 1, height - dif));

	}

	_backgroundLayer->setTileGID(FLOATEND, Vec2(++x, height - 2));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 2));

	return x + 3;

}

int TMXEdit::FloatingPlatform(int x, int height) {
	_backgroundLayer->setTileGID(FLOATSTART, Vec2(x, height - 2));
	_metaLayer->setTileGID(COLLIDE, Vec2(x, height - 2));
	int length = 2 + arc4random() % 3;
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
void TMXEdit::placeCoins(int distance) {
	int x = 100;
	while (x <  (map->getMapSize().width -100)) {
		int y = isSafe(x);
		if (y > 0) {
			placeCoin(x, y);
			x = x - (x % distance) + distance;
		} else { //TODO
			x += 1;
		}
	}

}
int TMXEdit::isSafe(int x) {
	int y = getHighestTile(x); //TODO
	if (y <= 0)
		return -1;
	int Gid = _metaLayer->getTileGIDAt(Vec2(x, y));
	CCLOG("%d,%d = PUNKT, %d = GID , is SAFE?", x, y, Gid);
	if (Gid == COLLIDE) {
		int s = checkSurroundings(x, y-COINHEIGHT);
		if (s > 0) {
			return y - COINHEIGHT;
		}
	}
	return -1;
}

int TMXEdit::getHighestTile(int x) {
	int ret = -1;
	for (int y = 14; y >= 0; y--) {
		int Gid = _backgroundLayer->getTileGIDAt(Vec2(x, y));

		CCLOG("%d,%d = PUNKT, %d = GID , is Highest?", x, y, Gid);
		if (Gid != 0)
			ret = y;
	}
	return ret;
}

int TMXEdit::checkSurroundings(int x, int y) {
	if (_backgroundLayer->getTileGIDAt(Vec2(x - 1, y)) == 0
			&& _backgroundLayer->getTileGIDAt(Vec2(x + 1, y)) == 0
			&& _backgroundLayer->getTileGIDAt(Vec2(x - 1, y+COINHEIGHT-1)) == 0
			&& _backgroundLayer->getTileGIDAt(Vec2(x + 1, y+COINHEIGHT-1)) == 0)
		return 1;
	return -1;
}

void TMXEdit::placeCoin(int x, int height) {
	_metaLayer->setTileGID(COIN, Vec2(x, height));
}
int TMXEdit::newHeight() {
	return MINHEIGHT - (arc4random() % (MINHEIGHT - MAXHEIGHT));
}
