/*
 * Cutscene.h
 *
 *  Created on: 21.04.2015
 *      Author: ryban
 */

#ifndef CUTSCENE_H_
#define CUTSCENE_H_

#include "cocos2d.h"

using namespace cocos2d;

class Cutscene : public cocos2d::Layer {
public:
	Cutscene();
	virtual ~Cutscene();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	    static cocos2d::Scene* createScene(int cutscene_number);

	    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	    virtual bool init();

	    // a selector callback
	    void menuCloseCallback(cocos2d::Ref* pSender);

	    // implement the "static create()" method manually
	    CREATE_FUNC(Cutscene);

		void play(cocos2d::Ref* pSender);
		void speech(cocos2d::Ref* pSender);
		void speech1(cocos2d::Ref* pSender);
		void speech2(cocos2d::Ref* pSender);
		void speech3(cocos2d::Ref* pSender);
		void speech4(cocos2d::Ref* pSender);
		/*void bossdefeated(cocos2d::Ref* pSender);*/
		void startlvl(cocos2d::Ref* pSender,int level, int sublevel);
		void placeStaticSprites();
		void talkSettings(int sub, bool speech);



private:
	int _cutscene_number;
	bool _speech;
	Sprite *_josie;
	MenuItemImage *_talk;
	bool _will_be_destroyed;

};

#endif /* CUTSCENE_H_ */
