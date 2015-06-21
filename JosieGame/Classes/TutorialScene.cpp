/*
 * TutorialScene.cpp
 *
 *  Created on: 21.06.2015
 *      Author: danielmugge
 */

#include "TutorialScene.h"

using namespace cocos2d;


TutorialScene::TutorialScene() {

	_move_counter = 0;
}

TutorialScene::~TutorialScene() {
}

TutorialScene* TutorialScene::initTutorial(){

	TutorialScene* tut = new TutorialScene();
	tut->init();
	tut->autorelease();

	tut->createTut();

	return tut;
}

void TutorialScene::createTut(){

	Layer* bgs = Layer::create();
	Sprite* bg = Sprite::create("backgrounds/bg_tut1.png");
	Sprite* bg_next = Sprite::create("backgrounds/bg_tut2.png");
	bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bg_next->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bg->setPosition(0,0);
	bg_next->setPosition(4065,4);
	bgs->addChild(bg);
	bgs->addChild(bg_next);
	this->addChild(bgs);

	Sprite* josie = Sprite::create("josie/josie_static.png");
	josie->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	josie->setPosition(200,50);
	this->addChild(josie);

	MenuItemImage* next = MenuItemImage::create(
										"buttons/right.png",
										"buttons/right.png",
										CC_CALLBACK_0(TutorialScene::nextMovement,this,bgs,josie));
	next->setPosition(1800,200);
	Menu* m = Menu::createWithItem(next);
	m->setPosition(Vec2::ZERO);
	this->addChild(m);







}

void TutorialScene::backToMenu(){
	Director::getInstance()->popScene();
}

void TutorialScene::nextMovement(Layer* layer,Sprite* josie){
	_move_counter++;
	std::ostringstream bg_str;
	switch(_move_counter){
	case 1:{
		MoveBy* move1 = MoveBy::create(1.5,Vec2(-1100,0));
		layer->runAction(move1);
	} break;
	case 2:{
		MoveBy* move2 = MoveBy::create(3.0,Vec2(-2000,0));
		layer->runAction(move2);
	} break;
	case 3:{
		MoveBy* move3 = MoveBy::create(3.0,Vec2(-2500,0));
		layer->runAction(move3);
	} break;
	case 4:{
		MoveBy* move4 = MoveBy::create(2.0,Vec2(-400,0));
		MoveBy* move_josie_home = MoveBy::create(2.0, Vec2(1700,0));
		CallFuncN* backToMenu = CallFuncN::create(CC_CALLBACK_0(TutorialScene::backToMenu,this));
		Sequence* lastOne = Sequence::createWithTwoActions(move_josie_home,backToMenu);
		layer->runAction(move4);
		josie->runAction(lastOne);
	}
	}


	//Sequence* walkthrough = Sequence::create(move1,DelayTime::create(5.0),move2,DelayTime::create(5.0),move3,nullptr);
	//layer->runAction(walkthrough);
}
