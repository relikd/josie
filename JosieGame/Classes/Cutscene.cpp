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
}
Cutscene::~Cutscene() {

}

Scene* Cutscene::createScene(int cutscene_number) {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	Cutscene* layer = Cutscene::create();

	// add layer as a child to scene
	scene->addChild(layer);
	layer->_cutscene_number = cutscene_number;
	if(cutscene_number == 1)
				layer->speech(nullptr);
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Cutscene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	return true;
}

void Cutscene::speech(Ref* pSender) {
	this->removeAllChildren();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

		// add "Cutscene" splash screen"
		auto background = Sprite::create("backgrounds/bg_mountain72dpi.png");
		background->setPosition(
				Vec2(visibleSize.width / 2 + origin.x,
						visibleSize.height / 2 + origin.y));
		this->addChild(background, 0);
		//add Ground
		auto testground = Sprite::create("testground.png");
		testground->setPosition(
				Vec2(origin.x + visibleSize.width / 2,
						origin.y + testground->getContentSize().height / 2));
		this->addChild(testground, 1);
	//create Josie
	Sprite* josie = Sprite::create("josie/josie_static.png");
	josie->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	josie->setPosition(Vec2(-100, 90));
	this->addChild(josie, 1);
	auto moveTo = MoveTo::create(1.5, Vec2(1350, 90));
	josie->runAction(moveTo);

	//add speechbubble
	auto talk = MenuItemImage::create("buttons/talk/talk1.1.png",
			"buttons/talk/talk1.1.png", CC_CALLBACK_1(Cutscene::speech1, this));
	FadeIn* fadeInText = FadeIn::create(3.0f);
	talk->setPosition(990, 800);
	auto menu = Menu::create(talk, NULL);
	menu->setPosition(Vec2::ZERO);
	talk->setScale(3.0);
	talk->setOpacity(0);
	talk->runAction(fadeInText);
	this->addChild(menu, 1);
}
void Cutscene::speech1(Ref* pSender) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->removeAllChildren();
	auto background = Sprite::create("backgrounds/bg_mountain72dpi.png");

	// position the sprite on the center of the screen
	background->setPosition(
			Vec2(visibleSize.width / 2 + origin.x,
					visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);

	auto testground = Sprite::create("testground.png");
	testground->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + testground->getContentSize().height / 2));
	this->addChild(testground, 1);

	auto talk = MenuItemImage::create("buttons/talk/talk1.2.png",
			"buttons/talk/talk1.2.png",
			CC_CALLBACK_1(Cutscene::startlvl, this, 1, 1));
	talk->setPosition(Vec2(940, 800));
	auto menu = Menu::create(talk, NULL);
	menu->setPosition(Vec2::ZERO);
	talk->setScale(3.0);
	this->addChild(menu, 1);
	FadeIn* fadeInText = FadeIn::create(2.0f);
	talk->setOpacity(0);
	talk->runAction(fadeInText);

	Sprite* josie = Sprite::create("josie/josie_static.png");
	josie->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	josie->setPosition(Vec2(1350, 90));
	this->addChild(josie, 1);
	auto moveTo = MoveTo::create(1, Vec2(500, 90));
	josie->runAction(moveTo);
}

void Cutscene::startlvl(Ref* pSender, int level, int sublevel) {
	auto levelxx = Level::initWithLevel(level, sublevel);
	Director::getInstance()->popScene();
	Director::getInstance()->pushScene(levelxx);
}


