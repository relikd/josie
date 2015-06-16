#include "PauseScreen.h"
#include "AudioUnit.h"

using namespace cocos2d;

PauseScreen::PauseScreen() : _overlay(nullptr) {}
PauseScreen::~PauseScreen() {
	CCLOG("~PauseScreen");
}

PauseScreen* PauseScreen::createPauseButton(float x, float y)
{
	PauseScreen *p = new PauseScreen();
	if (p->init())
	{
		MenuItemImage *pauseButton = MenuItemImage::create("buttons/pausebutton.png", "buttons/pausebutton.png", CC_CALLBACK_0(PauseScreen::pauseGame, p));
		Menu *holdon = Menu::createWithItem(pauseButton);
		holdon->setPosition(Vec2::ZERO);

		p->addChild(holdon);
		p->setPosition( Vec2(x,y) - pauseButton->getContentSize()/2 );

		p->autorelease();
		p->createPauseOverlay();
	}
	return p;
}

void PauseScreen::createPauseOverlay()
{
	_overlay = Layer::create();
	_overlay->setPosition( Vec2(470,175) - this->getPosition() );
	this->addChild(_overlay);

	Sprite *bg = Sprite::create("buttons/menubuttons/pausescreen.png");
	bg->setPosition(Vec2(0,0));
	bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	MenuItemImage *cont = MenuItemImage::create
			("buttons/menubuttons/resume.png",
			"buttons/menubuttons/resume.png",
			CC_CALLBACK_0(PauseScreen::continueGame, this));
	MenuItemImage *back = MenuItemImage::create
				("buttons/menubuttons/backtomenu.png",
				"buttons/menubuttons/backtomenu.png",
				CC_CALLBACK_0(PauseScreen::backToMenu, this));
	MenuItemImage *restart = MenuItemImage::create
					("buttons/menubuttons/restart.png",
					"buttons/menubuttons/restart.png"
					);
	cont->setPosition(bg->getContentSize().width/2, 290);
	back->setPosition(bg->getContentSize().width/4, 200);
	restart->setPosition(bg->getContentSize().width - (bg->getContentSize().width/4),200);


	Menu *m = Menu::create(cont, back,restart, nullptr);
	m->setPosition(Vec2::ZERO);

	_overlay->addChild(bg);
	bg->addChild(m);
	_overlay->setVisible(false);
}

void PauseScreen::pauseGame()
{
	if (!_overlay->isVisible()) {
		Director::getInstance()->pause();
		AudioUnit::pauseBackground();
		_overlay->setVisible(true);
	}
}

void PauseScreen::continueGame()
{
	Director::getInstance()->resume();
	AudioUnit::resumeBackground();
	_overlay->setVisible(false);
}

void PauseScreen::backToMenu()
{
	Director::getInstance()->resume();
	Director::getInstance()->popToSceneStackLevel(2);
}
