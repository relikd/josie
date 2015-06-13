/*
 * TMXEdit.cpp
 *
 *  Created on: 30.05.2015
 *      Author: Jonas
 */

#include "TMXEdit.h"
#include "MapController.h"

using namespace cocos2d;

const int minPartLength = 2;
const int minHeight = 17;
const int maxHeight = 11;
const int holeFreq = 2;//lower Value means more holes
const int maxHoleSize = 4;
//GIDs
const int topDirt = 1;
const int dirt = 17;
const int collide = 234;
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
	maker->fillGround(minHeight,maxHeight);
	maker->map->reinitializeMap();
	return maker->map;
}


void TMXEdit::getLayers(){
	_backgroundLayer = map->getLayer("Background_layer");
	_foregroundLayer = map->getLayer("Foreground_layer");
	_metaLayer = map->getLayer("Meta_layer");
}

void TMXEdit::fillGround(int ymin,int ymax){
	int y = ymin;
	int startNext = minPartLength;
	int width = map->getMapSize().width;
	for (int x = 20; x < width; ++x)
	{
		placeGround(x,y);


		if (x%startNext==0) //bestimmt wann eine neue H�he ermittelt werden soll
		{
			y = ymin - (arc4random()%(ymin-ymax)); // bestimmt die neue H�he
			startNext =  minPartLength+(arc4random()%minPartLength);
		}
		else if (arc4random()%holeFreq == 0 ){
					x+= 2 + (arc4random()%maxHoleSize);
				}
	}
}

void TMXEdit::placeGround(int x, int y){

	_backgroundLayer->setTileGID(topDirt,Vec2(x,y));
	_metaLayer->setTileGID(collide,Vec2(x,y));
	if ( y < minHeight )placeDirt(x, y+1);
}

void TMXEdit::placeDirt(int x, int y){
	_backgroundLayer->setTileGID(dirt,Vec2(x,y));
	_metaLayer->setTileGID(collide,Vec2(x,y));
	if ( y < minHeight )placeDirt(x, y+1);
}
