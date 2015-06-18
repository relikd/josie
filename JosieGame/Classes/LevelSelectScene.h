/*
 * LevelSelectScene.h
 *
 *  Created on: 25.04.2015
 *      Author: danielmugge
 */

#ifndef LEVELSELECTSCENE_H_
#define LEVELSELECTSCENE_H_

#include "cocos2d.h"


class LevelSelect : public cocos2d::Layer{
public:
	LevelSelect();
	virtual ~LevelSelect();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void buildUI();
    void backToMainMenu(cocos2d::Ref* pSender);
    void startLevel(cocos2d::Ref* pSender,int level, int sublevel);
    void testTalk(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(LevelSelect);
};


#endif /* LEVELSELECTSCENE_H_ */
