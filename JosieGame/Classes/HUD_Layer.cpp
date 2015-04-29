/*
 * HUD_Layer.cpp
 *
 *  Created on: 29.04.2015
 *      Author: Jonas
 */

#include "HUD_Layer.h"
#include "Level.h"
#include "PlayerControl.h"

namespace cocos2d {


Size visibleSize;
Vec2 origin;


HUD_Layer::HUD_Layer() {
	// TODO Auto-generated constructor stub

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
}

HUD_Layer::~HUD_Layer() {
	// TODO Auto-generated destructor stub
}

} /* namespace cocos2d */



HUD_Layer* HUD_Layer::createForLevel(Level* level)
{
	HUD_Layer* toCreate = new HUD_Layer();

	toCreate->addBackground(level);
	toCreate->addPauseButton(level);
	toCreate->addPlayerControl(level);
	return toCreate;
}

void HUD_Layer::addBackground(Level* level){
	//index to string for background loading
	std::ostringstream s;
	s << "backgrounds/bg_" << level->currentLevel << "." << level->currentSubLevel << ".png";

	//Add Background Image
	auto background = Sprite::create(s.str());
	background->setPosition(
			Vec2(visibleSize.width / 2 + origin.x,
					visibleSize.height / 2 + origin.y));
	this->addChild(background, 0);
}


void HUD_Layer::addPauseButton(Level* level){
	//Add Pause Button in upper right corner
		auto pause = MenuItemImage::create("buttons/pausebutton.png",
				"buttons/pausebutton.png", CC_CALLBACK_1(Level::pause, level));
		pause->setPosition(
				origin.x + visibleSize.width - pause->getContentSize().width,
				origin.y + visibleSize.height - pause->getContentSize().height);
		auto menu = Menu::create(pause, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
}


void HUD_Layer::addPlayerControl(Level* level){

	auto playerControl = PlayerControl::initWithLevel(level);
	this->addChild(playerControl, -1);
}
