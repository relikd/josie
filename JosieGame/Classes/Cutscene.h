/*
 * Cutscene.h
 *
 *  Created on: 21.04.2015
 *      Author: ryban
 */

#ifndef CUTSCENE_H_
#define CUTSCENE_H_

#include "cocos2d.h"



class Cutscene : public cocos2d::Layer {
public:
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
		void startlvl(cocos2d::Ref* pSender,int level, int sublevel);




	Cutscene();
	virtual ~Cutscene();
private:
	int _cutscene_number;
};

#endif /* CUTSCENE_H_ */
