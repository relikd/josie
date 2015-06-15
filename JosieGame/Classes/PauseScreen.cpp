#include "PauseScreen.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
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
	_overlay->setPosition( Vec2(560,100) - this->getPosition() );
	this->addChild(_overlay);

	LayerColor *bg = LayerColor::create(Color4B::BLACK, 800, 880);
	bg->setOpacity(128);

	Label *txt_continue = Label::createWithTTF("Continue", "fonts/Marker Felt.ttf", 80);
	Label *txt_backmenu = Label::createWithTTF("Back to Menu", "fonts/Marker Felt.ttf", 80);
	MenuItemLabel *cont = MenuItemLabel::create(txt_continue, CC_CALLBACK_0(PauseScreen::continueGame, this));
	MenuItemLabel *back = MenuItemLabel::create(txt_backmenu, CC_CALLBACK_0(PauseScreen::backToMenu, this));
	cont->setPosition(400, 760);
	back->setPosition(400, 600);

	Menu *m = Menu::create(cont, back, nullptr);
	m->setPosition(Vec2::ZERO);

	_overlay->addChild(bg);
	_overlay->addChild(m);
	_overlay->setVisible(false);
}

void PauseScreen::pauseGame()
{
	if (!_overlay->isVisible()) {
		Director::getInstance()->pause();
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		_overlay->setVisible(true);
	}
}

void PauseScreen::continueGame()
{
	Director::getInstance()->resume();
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	_overlay->setVisible(false);
}

void PauseScreen::backToMenu()
{
	Director::getInstance()->resume();
	Director::getInstance()->popToSceneStackLevel(2);
}
