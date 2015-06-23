/*
 * TMXEdit.h
 *
 *  Created on: 30.05.2015
 *      Author: Jonas
 */

#ifndef TMXEDIT_H_
#define TMXEDIT_H_

#include "cocos2d.h"

class MapController;

using namespace cocos2d ;

class TMXEdit {
public:
	TMXEdit();
	virtual ~TMXEdit();
	static MapController* makeMap(int difficulty);
	void getLayers();
	void fillLevel();
	void placeGround(int x, int y);
	void placeDirt(int x, int y);
	int makePillars(int x, int height);
	int makeThornField(int x, int height);
	int makeFloating(int x ,int height);
	int makeSlide(int x, int height);
	int makeSpikeSlide(int x, int height);
	int makeJumpAndSlide(int x,int height);
	int makeWater(int x, int height);
	int FloatingPlatform(int x,int height, int length);
	int placeGroundLength(int x, int height, int length);
	void placeHazards(int distance);
	void placeCoins(int number);
	int isSafe(int x);
	int getHighestTile(int x);
	int checkSurroundings(int x,int y);
	void placeCoin(int x,int height);
	int newHeight();

	MapController *map;

private:
	TMXLayer* _backgroundLayer;
	TMXLayer* _metaLayer;
	TMXLayer* _foregroundLayer;

	int _partlength;
	int _minheight;
	int _maxheight;
	int _coinheight;
	int _hazardDistanceMin;
	int _coins;
	int _minJumpDistance;
	int _maxJumpDistance;
};


#endif /* TMXEDIT_H_ */
