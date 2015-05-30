/*
 * TMXEdit.h
 *
 *  Created on: 30.05.2015
 *      Author: Jonas
 */

#ifndef TMXEDIT_H_
#define TMXEDIT_H_

#include "cocos2d.h"

using namespace cocos2d ;

class TMXEdit {
public:
	TMXEdit();
	virtual ~TMXEdit();
	static cocos2d::TMXTiledMap* makeMap();
	void getLayers();
	void fillGround(int ymin,int ymax);
	void placeGround(int x, int y);
	void placeDirt(int x, int y);

	cocos2d::TMXTiledMap *map;

private:
	TMXLayer* _backgroundLayer;
	TMXLayer* _metaLayer;
	TMXLayer* _foregroundLayer;
};


#endif /* TMXEDIT_H_ */
