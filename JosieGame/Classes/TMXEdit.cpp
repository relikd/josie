/*
 * TMXEdit.cpp
 *
 *  Created on: 30.05.2015
 *      Author: Jonas
 */

#include "TMXEdit.h"
#include "MapController.h"

using namespace cocos2d;

const int PartLength = 6;
const int minHeight = 14;
const int maxHeight = 8;

//GIDs
const int TOPDIRT[] = {1,2,3,4};
const int DIRT[] = {5,6,7,8};
const int COLLIDE = 20;
const int THORN = 24;
const int FLOATSTART= 9;
const int FLOATEND = 12;
const int FLOATING[]= {10,11,13,14};
TMXEdit::TMXEdit() {
	// TODO Auto-generated constructor stub
		map = MapController::initWithLevel(0,1);
		_backgroundLayer=NULL;
		_metaLayer=NULL;
		_foregroundLayer=NULL;
}

TMXEdit::~TMXEdit() {
	// TODO Auto-generated destructor stub
}

//TODO: erschaffe map mit parametern
MapController* TMXEdit::makeMap(){
	TMXEdit* maker = new TMXEdit();
	maker->getLayers();
	maker->fillLevel();
	maker->map->reinitializeMap();
	return maker->map;
}


void TMXEdit::getLayers(){
	_backgroundLayer = map->getLayer("Background_layer");
	_foregroundLayer = map->getLayer("Foreground_layer");
	_metaLayer = map->getLayer("Meta_layer");
}

void TMXEdit::fillLevel(){
	int width = map->getMapSize().width;
	int x = 20;
	int height = 13;
	while (x < width-(4*PartLength)){

		switch(arc4random()%6)
		{
		case 0:
			x = makePillars(x, height);
			x = placeGroundLength(x, height,2+arc4random()%5);
			break;

		case 1:
			height = newHeight();
			x = placeGroundLength(x,height,8);
		    break;

		case 2:
			x = makeThornField(x,height);
			break;

		case 3:
			x=makeFloating(x, height);
			x = placeGroundLength(x, height,2+arc4random()%5);
			break;

		case 4:
			break;

		case 5:
			break;
		}
	}
	while(x < width){
		placeGround(x, height);
		x++;
	}
}

void TMXEdit::placeGround(int x, int y){

	_backgroundLayer->setTileGID(TOPDIRT[arc4random()%4],Vec2(x,y));
	_metaLayer->setTileGID(COLLIDE,Vec2(x,y));
	if ( y < minHeight )placeDirt(x, y+1);
}

void TMXEdit::placeDirt(int x, int y){
	_backgroundLayer->setTileGID(DIRT[arc4random()%4],Vec2(x,y));
	_metaLayer->setTileGID(COLLIDE,Vec2(x,y));
	if ( y < minHeight )placeDirt(x, y+1);
}

int TMXEdit::makePillars(int x, int height){
	height-=3;
	x+=2;
	int done = x + PartLength*(1+arc4random()%2);
	while(x <= done){
		placeGround(x++, height);
		placeGround(x++, height);

		x+= 2 + (arc4random()%5);
	}

	return x;
}

int TMXEdit::makeThornField(int x, int height){
	int done = x + PartLength*(1+arc4random()%2);
	while(x <= done){
		x= placeGroundLength(x,height,(2+arc4random()%2));
		_backgroundLayer->setTileGID(THORN,Vec2(x,height-1)); //TODO: eventuell Collision für tödliche Spikes ändern
		x= placeGroundLength(x,height,2+arc4random()%3);
	}

	return x;
}

int TMXEdit::makeFloating(int x, int height){
	x+=2+arc4random()%3;
	int done = x + PartLength*(1+arc4random()%2);
	while(x <= done){
		x=FloatingPlatform(x, height);
		x+=3+arc4random()%3;
	}

	return x;

}

int TMXEdit::FloatingPlatform(int x, int height){
	_backgroundLayer->setTileGID(FLOATSTART,Vec2(x,height-2));
	_metaLayer->setTileGID(COLLIDE,Vec2(x,height-2));
	int length = 2 + arc4random()%3;
	while (length-- > 0){
		_backgroundLayer->setTileGID(FLOATING[arc4random()%4],Vec2(++x,height-2));
		_metaLayer->setTileGID(COLLIDE,Vec2(x,height-2));
	}
	_backgroundLayer->setTileGID(FLOATEND,Vec2(++x,height-2));
	_metaLayer->setTileGID(COLLIDE,Vec2(x,height-2));
	return x;

}

int TMXEdit::placeGroundLength(int x, int height, int length){
	int done = x + length;
	while(x <= done) placeGround(x++, height);

	return x;
}

int TMXEdit::newHeight(){
	return minHeight-(arc4random()%(minHeight-maxHeight));
}
