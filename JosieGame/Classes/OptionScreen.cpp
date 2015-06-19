/*
 * OptionScreen.cpp
 *
 *  Created on: 16.06.2015
 *      Author: danielmugge
 */

#include "OptionScreen.h"
#include "ui/CocosGUI.h"
#include "MainMenuScene.h"

using namespace cocos2d;

OptionScreen::OptionScreen() {
	_overlay = nullptr;
	_music_volume_slider = nullptr;
	_sfx_volume_slider = nullptr;
}

OptionScreen::~OptionScreen() {
	// TODO Auto-generated destructor stub
}

OptionScreen* OptionScreen::createOptionButton(float x, float y)
{
	OptionScreen *os = new OptionScreen();

	if(os->init()){
		MenuItemImage *optionButton = MenuItemImage::create(
				"buttons/menubuttons/optionsbutton.png",
				"buttons/menubuttons/optionsbutton.png",
				CC_CALLBACK_0(OptionScreen::toggleVisibility,os));
		Menu* show = Menu::createWithItem(optionButton);
		show->setPosition(Vec2::ZERO);

		os->addChild(show);
		os->setPosition(Vec2(x - optionButton->getContentSize().width/2, y + optionButton->getContentSize().height/2));

		os->autorelease();
		os->createOptionOverlay();
	}
	return os;
}

void OptionScreen::createOptionOverlay(){


	_overlay = Layer::create();
	_overlay->setPosition(Vec2(470, 175) - this->getPosition());
	this->addChild(_overlay);

	Sprite *bg = Sprite::create("buttons/menubuttons/optionscreen.png");
	bg->setPosition(Vec2(0, 0));
	bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	Label *back = Label::createWithTTF("Back","fonts/Marker Felt.ttf", 72);
	back->setColor(Color3B::BLACK);
	MenuItemLabel *backTo = MenuItemLabel::create(back, CC_CALLBACK_0(OptionScreen::toggleVisibility,this));
	backTo->setPosition(bg->getContentSize().width - 120,80);


	//Slider for Music Volume
	_music_volume_slider = ui::Slider::create(
			"buttons/menubuttons/optionslidertrail.png",
			"buttons/menubuttons/optionslider.png",
			ui::TextureResType::LOCAL);
	_music_volume_slider->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2 + 48));
	_music_volume_slider->setPercent(UserDefault::getInstance()->getIntegerForKey("music_volume"));

	//Slider for SFX Volume
	_sfx_volume_slider = ui::Slider::create(
			"buttons/menubuttons/optionslidertrail.png",
			"buttons/menubuttons/optionslider.png",
			ui::TextureResType::LOCAL);
	_sfx_volume_slider->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2 - 120));
	_sfx_volume_slider->setPercent(UserDefault::getInstance()->getIntegerForKey("sfx_volume"));



	Menu *m = Menu::createWithItem(backTo);
	m->setPosition(Vec2::ZERO);

	_overlay->addChild(bg);
	bg->addChild(m);
	bg->addChild(_music_volume_slider);
	bg->addChild(_sfx_volume_slider);

	_overlay->setVisible(false);
}

void OptionScreen::toggleVisibility(){
	if(_overlay->isVisible()){
		_overlay->setVisible(false);
		UserDefault::getInstance()->setIntegerForKey("music_volume", _music_volume_slider->getPercent());
		UserDefault::getInstance()->setIntegerForKey("sfx_volume", _sfx_volume_slider->getPercent());
		((Menu*)(this->getParent()->getChildByTag(101)))->setEnabled(true);
	}
	else{
		_overlay->setVisible(true);
		((Menu*)(this->getParent()->getChildByTag(101)))->setEnabled(false);
	}

	CCLOG("music volume: %d", UserDefault::getInstance()->getIntegerForKey("music_volume"));
	CCLOG("sfx volume: %d", UserDefault::getInstance()->getIntegerForKey("sfx_volume"));
}

