/*
 * TutorialScene.h
 *
 *  Created on: 21.06.2015
 *      Author: danielmugge
 */

#ifndef TUTORIALSCENE_H_
#define TUTORIALSCENE_H_

#include "cocos2d.h"

class TutorialScene : public cocos2d::Scene {
public:
	TutorialScene();
	virtual ~TutorialScene();

	static TutorialScene* initTutorial();

private:

	void createTut();
	void backToMenu();
	void nextMovement(cocos2d::Layer* layer,cocos2d::Sprite* josie);
	int _move_counter;

};


#endif /* TUTORIALSCENE_H_ */
