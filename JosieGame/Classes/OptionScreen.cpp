#include "OptionScreen.h"
#include "ui/CocosGUI.h"
#include "MainMenuScene.h"
#include "GameStateManager.h"

using namespace cocos2d;

OptionScreen::OptionScreen() {
	_overlay = nullptr;
	_music_volume_slider = nullptr;
	_sfx_volume_slider = nullptr;
}
OptionScreen::~OptionScreen() {}

OptionScreen* OptionScreen::createOptionButton(float x, float y)
{
	OptionScreen *os = new OptionScreen();

	if(os->init()){
		MenuItemImage *optionButton = MenuItemImage::create(
				"buttons/menubuttons/optionsbutton.png",
				"buttons/menubuttons/optionsbutton.png",
				CC_CALLBACK_0(OptionScreen::toggleVisibility,os));
		optionButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		optionButton->setScale(3.0);
		RotateBy* rotate = RotateBy::create(20.0,360);
		RepeatForever* rotate_forever = RepeatForever::create(rotate);
		optionButton->runAction(rotate_forever);
		Menu* show = Menu::createWithItem(optionButton);
		show->setPosition(Vec2::ZERO);

		os->addChild(show);
		os->setPosition(Vec2(x - optionButton->getContentSize().width/2, y + optionButton->getContentSize().height/2));

		os->autorelease();
		os->createOptionOverlay();
	}
	return os;
}

void OptionScreen::createOptionOverlay()
{
	_overlay = Layer::create();
	_overlay->setPosition( -this->getPosition() );
	this->addChild(_overlay);

	Sprite *bg = Sprite::create("buttons/menubuttons/optionscreen.png");
	bg->setPosition(Vec2(470, 175));
	bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	Label *reset_game = Label::createWithTTF("Reset Game","fonts/Marker Felt.ttf", 72);
	reset_game->setColor(Color3B::BLACK);
	MenuItemLabel *reset_button = MenuItemLabel::create(reset_game, CC_CALLBACK_0(OptionScreen::resetGameState,this));
	reset_button->setPosition(200,80);

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



	Menu *m = Menu::create(reset_button,nullptr);
	m->setPosition(Vec2::ZERO);

	_overlay->addChild(bg);
	bg->addChild(m);
	bg->addChild(_music_volume_slider);
	bg->addChild(_sfx_volume_slider);
	
	if (JOSIE_DEVELOPER_MODE) {
		addDeveloperControls();
	}

	_overlay->setVisible(false);
}

void OptionScreen::addDeveloperControls()
{
	Label *lbl_unlock = Label::createWithTTF("Unlock All Levels", "fonts/Marker Felt.ttf", 70);
	Label *lbl_money = Label::createWithTTF("Money = 9001", "fonts/Marker Felt.ttf", 70);
	Label *lbl_cutscene = Label::createWithTTF("No Cutscenes", "fonts/Marker Felt.ttf", 70);
	
	MenuItemLabel *unlock = MenuItemLabel::create(lbl_unlock, CC_CALLBACK_0(UserDefault::setStringForKey, UserDefault::getInstance(), "josie_collected_coins", "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"));
	unlock->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	unlock->setPosition(30, Director::getInstance()->getWinSize().height-50);
	
	MenuItemLabel *money = MenuItemLabel::create(lbl_money, CC_CALLBACK_0(UserDefault::setIntegerForKey, UserDefault::getInstance(), "josie_credits", 9001));
	money->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	money->setPosition(unlock->getPosition()-Vec2(0,120));
	
	MenuItemLabel *cutscene = MenuItemLabel::create(lbl_cutscene, CC_CALLBACK_0(UserDefault::setBoolForKey, UserDefault::getInstance(), "josie_no_cutscenes", true));
	cutscene->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	cutscene->setPosition(money->getPosition()-Vec2(0,120));
	
	Menu *m = Menu::create(unlock, money, cutscene, NULL);
	m->setPosition(Vec2::ZERO);
	m->setColor(Color3B::BLACK);
	
	_overlay->addChild(m);
}

void OptionScreen::resetGameState(){
	GameStateManager::initDefaults(true);
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
}

