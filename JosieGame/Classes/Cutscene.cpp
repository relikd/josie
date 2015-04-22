/*
 * Cutscene.cpp
 *
 *  Created on: 21.04.2015
 *      Author: ryban
 */

#include "Cutscene.h"
#include <ui/UIText.h>
USING_NS_CC;

Cutscene::Cutscene() {

}
Cutscene::~Cutscene() {

}

Scene* Cutscene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Cutscene::create();

	// add layer as a child to scene
	scene->addChild(layer);

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

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add "Cutscene" splash screen"
	auto background = Sprite::create("backgrounds/bg_mountain72dpi.png");

	// position the sprite on the center of the screen
	background->setPosition(
			Vec2(visibleSize.width / 2 + origin.x,
					visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer

	this->addChild(background, 0);

	auto textbox = Sprite::create("textbox.png");
	textbox->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height
							- textbox->getContentSize().height * 3 - 20));
	background->addChild(textbox, 1);
	textbox->setScale(3.0);

	auto label1 = Label::createWithTTF("Josie hat Hunger",
			"fonts/Marker Felt.ttf", 36);
	label1->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height
							- textbox->getContentSize().height * 3 - 20));

	label1->enableShadow();
	label1->setColor(Color3B::BLACK);
	label1->setScale(2.0);
	textbox->addChild(label1);

	return true;

}

