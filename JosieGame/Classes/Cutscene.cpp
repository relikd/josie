/*
 * Cutscene.cpp
 *
 *  Created on: 21.04.2015
 *      Author: ryban
 */

#include "Cutscene.h"
#include <ui/UIText.h>
#include "MainMenuScene.h"
#include "Level.h"

USING_NS_CC;

Cutscene::Cutscene() {
	_cutscene_number = 0;
	_will_be_destroyed = false;
	_speech = false;
	_josie = nullptr;
	_talk = nullptr;
}
Cutscene::~Cutscene() {}

Scene* Cutscene::createScene(int cutscene_number) {
	auto scene = Scene::create();
	Cutscene* layer = Cutscene::create();
	scene->addChild(layer);
	layer->_cutscene_number = cutscene_number;
	switch (cutscene_number) {
	case 1: {
		layer->speech(nullptr);
		break;
	}
	case 2: {
		layer->speech2(nullptr);
		break;
	}
	case 3: {
		layer->speech4(nullptr);
		break;
	}
		/*case 0:
		 {
		 layer->bossdefeated(nullptr);
		 break;
		 }*/
	}
	return scene;
}
bool Cutscene::init() {
	if (!Layer::init()) {
		return false;
	}
	return true;
}

void Cutscene::speech(Ref* pSender) { //Cutscene for Lvl1.1 Part1
	Cutscene::placeStaticSprites();
	_josie->setPosition(Vec2(-100, 90));
	MoveTo* moveTo = MoveTo::create(1, Vec2(500, 90));
	_josie->runAction(moveTo);

	Cutscene::talkSettings(1, true);
	FadeIn* fadeInText = FadeIn::create(2.0f);
	_talk->runAction(fadeInText);
	_talk->setOpacity(0);
}
void Cutscene::speech1(Ref* pSender) { //Cutscene for Lvl1.1 Part2
	Cutscene::placeStaticSprites();
	_josie->setPosition(Vec2(500, 90));

	Cutscene::talkSettings(1, false);
}
void Cutscene::speech2(Ref* pSender) { //Cutscene for Lvl1.2 Part1
	Cutscene::placeStaticSprites();
	_josie->setPosition(Vec2(-100, 90));
	MoveTo* moveTo = MoveTo::create(1, Vec2(500, 90));
	RotateTo* rotateToLeft = RotateTo::create(0.2f, 40.0f);
	RotateTo* rotateToRight = RotateTo::create(0.2f, -40.0f);
	RotateTo* rotateToMid = RotateTo::create(0.2f, 0.0f);
	Sequence* action = Sequence::create(moveTo, rotateToLeft, rotateToRight,
			rotateToLeft, rotateToRight, rotateToMid, nullptr);
	_josie->runAction(action);

	Cutscene::talkSettings(2, true);
	FadeIn* fadeInText = FadeIn::create(2.0f);
	_talk->setOpacity(0);
	_talk->runAction(fadeInText);

}
void Cutscene::speech3(Ref* pSender) { //Cutscene for Lvl1.2 Part2
	Cutscene::placeStaticSprites();
	_josie->setPosition(Vec2(500, 90));
	MoveTo* moveTo = MoveTo::create(1, Vec2(2020, 90));
	_josie->runAction(moveTo);

	Cutscene::talkSettings(2, false);

}
void Cutscene::speech4(Ref* pSender) { //Cutscene for Lvl1.3
	Cutscene::placeStaticSprites();
	_josie->setPosition(Vec2(-100, 90));
	auto moveTo = MoveTo::create(1.5, Vec2(2020, 90));
	_josie->runAction(moveTo);

	Cutscene::talkSettings(3, false);
}
/*void Cutscene::bossdefeated(Ref* pSender){
 Cutscene::placeStaticSprites();
 }*/

void Cutscene::startlvl(Ref* pSender, int level, int sublevel) {
	if (_will_be_destroyed)
		return;
	_will_be_destroyed = true;
	auto levelxx = Level::initWithLevel(level, sublevel);
	Director::getInstance()->replaceScene(levelxx);
}

void Cutscene::placeStaticSprites() {
	this->removeAllChildren();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("backgrounds/bg_1.3.png");
	background->setPosition(
			Vec2(visibleSize.width / 2 + origin.x,
					visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);

	auto testground = Sprite::create("testground.png");
	testground->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + testground->getContentSize().height / 2));
	this->addChild(testground, 1);

	_josie = Sprite::create("josie/josie_static.png");
	_josie->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	_josie->setScale(0.55);
	this->addChild(_josie, 1);
}
void Cutscene::talkSettings(int sub, bool _speech) {

	std::ostringstream btn_str;
	btn_str << "buttons/talk/talk" << _cutscene_number << "." << sub << ".png";
	if (!_speech) {
		_talk = MenuItemImage::create(btn_str.str(), btn_str.str(),
				CC_CALLBACK_1(Cutscene::startlvl, this, 1, sub));
	} else {
		switch (_cutscene_number) {
		case 1: {
			_talk = MenuItemImage::create(btn_str.str(), btn_str.str(),
					CC_CALLBACK_1(Cutscene::speech1, this));
			break;
			}
		case 2: {
			_talk = MenuItemImage::create(btn_str.str(), btn_str.str(),
					CC_CALLBACK_1(Cutscene::speech3, this));
			break;
			}
		}
	}
	_talk->setPosition(Vec2(940, 800));
	Menu* menu = Menu::create(_talk, NULL);
	menu->setPosition(Vec2::ZERO);
	_talk->setScale(3.0);
	this->addChild(menu, 1);
}
