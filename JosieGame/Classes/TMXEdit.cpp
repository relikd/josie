/*
 * TMXEdit.cpp
 *
 *  Created on: 30.05.2015
 *      Author: Jonas
 */

#include "TMXEdit.h"

using namespace cocos2d;

const int minPartLength = 1;
const int minHeight = 8;
const int maxHeight = 5;
const int holeFreq = 4;//lower Value means more holes
const int maxHoleSize = 2;
//GIDs
const int gras = 6;
const int topDirt = 1;
const int dirt = 2;
const int collide = 8;
TMXEdit::TMXEdit() {
	// TODO Auto-generated constructor stub
		std::ostringstream mapstr;
		mapstr << "tilemaps/0.1.tmx";
		map = TMXTiledMap::create(mapstr.str());

		_backgroundLayer=NULL;
		_metaLayer=NULL;
		_foregroundLayer=NULL;
}

TMXEdit::~TMXEdit() {
	// TODO Auto-generated destructor stub
}

//TODO: erschaffe map mit parametern
TMXTiledMap* TMXEdit::makeMap(){
	TMXEdit* maker = new TMXEdit();
	maker->getLayers();
	maker->fillGround(minHeight,maxHeight);
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
	for (int x = 10; x < width; ++x)
	{
		placeGround(x,y);


		if (x%startNext==0) //bestimmt wann eine neue Höhe ermittelt werden soll
		{
			y = ymin - (arc4random()%(ymin-ymax)); // bestimmt die neue Höhe
			startNext =1 + x + minPartLength+(arc4random()%minPartLength);
		}
		else if (arc4random()%holeFreq == 0 ){
					x+= 2 + (arc4random()%maxHoleSize);
				}
	}
}

void TMXEdit::placeGround(int x, int y){

	_backgroundLayer->setTileGID(topDirt,Vec2(x,y));
	_backgroundLayer->setTileGID(gras,Vec2(x,y-1));
	_metaLayer->setTileGID(collide,Vec2(x,y));
	if ( y < 8 )placeDirt(x, y+1);
}

void TMXEdit::placeDirt(int x, int y){
	_backgroundLayer->setTileGID(dirt,Vec2(x,y));
	_metaLayer->setTileGID(collide,Vec2(x,y));
	if ( y < 8 )placeDirt(x, y+1);
}
