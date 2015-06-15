#include "PauseScreen.h"

using namespace cocos2d;

PauseScreen::PauseScreen() {}
PauseScreen::~PauseScreen() {
	CCLOG("~PauseScreen");
}

PauseScreen* PauseScreen::create()
{
	PauseScreen *p = new PauseScreen();
	p->autorelease();
	p->createUI();
	return p;
}

void PauseScreen::createUI()
{
	LayerColor *bg = LayerColor::create(Color4B::BLACK, 800, 900);
	bg->setOpacity(128);
	bg->setPosition(560, 50);

	Label *txt_continue = Label::createWithTTF("Continue", "fonts/Marker Felt.ttf", 80);
	Label *txt_backmenu = Label::createWithTTF("Back to Menu", "fonts/Marker Felt.ttf", 80);
	MenuItemLabel *cont = MenuItemLabel::create(txt_continue, CC_CALLBACK_0(PauseScreen::continueGame, this));
	MenuItemLabel *back = MenuItemLabel::create(txt_backmenu, CC_CALLBACK_0(PauseScreen::backToMenu, this));
	cont->setPosition(400, 800);
	back->setPosition(400, 600);

	Menu *m = Menu::create(cont, back, nullptr);
	m->setPosition(bg->getPosition());

	this->addChild(bg);
	this->addChild(m);

	Director::getInstance()->pause();
}

void PauseScreen::continueGame()
{
	Director::getInstance()->resume();
	this->removeFromParent();
}

void PauseScreen::backToMenu()
{
	Director::getInstance()->resume();
	Director::getInstance()->popToSceneStackLevel(2);
}
