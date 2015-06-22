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
	switch(cutscene_number){
	case 1:
	{
		layer->speech(nullptr);
		break;
	}
	case 2:
	{
		layer->speech2(nullptr);
		break;
	}
	case 3:
	{
		layer->speech4(nullptr);
		break;
	}
	/*case 0:
	{
		layer->bossdefeated(nullptr);
		break;
	}*/
	}


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
		auto background = Sprite::create("backgrounds/bg_1.1.png");
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
	auto background = Sprite::create("backgrounds/bg_1.1.png");

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
void Cutscene::speech2(Ref* pSender) {
	this->removeAllChildren();
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

			// add "Cutscene" splash screen"
			auto background = Sprite::create("backgrounds/bg_1.2.png");
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
		josie->setScale(0.55);
		this->addChild(josie, 1);
		MoveTo* moveTo = MoveTo::create(1, Vec2(500, 90));
		RotateTo* rotateToLeft = RotateTo::create(0.2f,40.0f);
		RotateTo* rotateToRight = RotateTo::create(0.2f, -40.0f);
		RotateTo* rotateToMid = RotateTo::create(0.2f, 0.0f);
		Sequence* action = Sequence::create(moveTo, rotateToLeft, rotateToRight, rotateToLeft, rotateToRight, rotateToMid, nullptr);
		josie->runAction(action);

		MenuItemImage* talk = MenuItemImage::create("buttons/talk/talk1.2.png",
					"buttons/talk/talk1.2.png",
					CC_CALLBACK_1(Cutscene::speech3, this));
			talk->setPosition(Vec2(940, 800));
			auto menu = Menu::create(talk, NULL);
			menu->setPosition(Vec2::ZERO);
			talk->setScale(3.0);
			this->addChild(menu, 1);
			FadeIn* fadeInText = FadeIn::create(2.0f);
			talk->setOpacity(0);
			talk->runAction(fadeInText);

}
void Cutscene::speech3(Ref* pSender){
	this->removeAllChildren();
	Size visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();

				// add "Cutscene" splash screen"
				auto background = Sprite::create("backgrounds/bg_1.2.png");
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
			josie->setPosition(Vec2(500, 90));
			MoveTo* moveTo = MoveTo::create(1 , Vec2(2020, 90));
			josie->runAction(moveTo);
			josie->setScale(0.55);
			this->addChild(josie, 1);

			MenuItemImage* talk = MenuItemImage::create("buttons/talk/talk1.2.png",
						"buttons/talk/talk1.2.png",
						CC_CALLBACK_1(Cutscene::startlvl, this, 1, 2));
				talk->setPosition(Vec2(940, 800));
				auto menu = Menu::create(talk, NULL);
				menu->setPosition(Vec2::ZERO);
				talk->setScale(3.0);
				this->addChild(menu, 1);


}
void Cutscene::speech4(Ref* pSender){
	this->removeAllChildren();
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

			// add "Cutscene" splash screen"
			auto background = Sprite::create("backgrounds/bg_1.3.png");
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
		auto moveTo = MoveTo::create(1.5, Vec2(2020, 90));
		josie->runAction(moveTo);

		//add speechbubble
		auto talk = MenuItemImage::create("buttons/talk/talk1.1.png",
				"buttons/talk/talk1.1.png", CC_CALLBACK_1(Cutscene::startlvl, this,1 ,3));
		//FadeIn* fadeInText = FadeIn::create(3.0f);
		talk->setPosition(990, 800);
		auto menu = Menu::create(talk, NULL);
		menu->setPosition(Vec2::ZERO);
		talk->setScale(3.0);
		this->addChild(menu, 1);

}
/*void Cutscene::bossdefeated(Ref* pSender){
	this->removeAllChildren();
			Size visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();

				// add "Cutscene" splash screen"
				auto background = Sprite::create("backgrounds/bg_1.0.png");
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
			josie->setPosition(Vec2(500, 90));
			this->addChild(josie, 1);
			auto moveTo = MoveTo::create(1.5, Vec2(500, 180));
			Sequence* jump = Sequence::create(moveTo, moveTo, moveTo, nullptr);
			josie->runAction(jump);

}*/


void Cutscene::startlvl(Ref* pSender, int level, int sublevel) {
	auto levelxx = Level::initWithLevel(level, sublevel);
	Director::getInstance()->replaceScene(levelxx);
}


