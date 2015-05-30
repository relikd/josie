#include "PlayerControl.h"
#include "Level.h"
#include "Player.h"
#include "PlayerBoss.h"


using namespace cocos2d;



PlayerControl::PlayerControl() {
	this->_level = NULL;
	this->_counterForShoot = 0;
	this->_listenerLevel = NULL;
	this->_left = NULL;
	this->_right = NULL;
	this->_shoot = NULL;
	this->_stay = NULL;
	this->_slide = NULL;
}
PlayerControl::~PlayerControl() {
	this->unscheduleUpdate();
	CCLOG("PlayerControl destroyed");
}

PlayerControl* PlayerControl::initWithLevel(Level* level)
{
	PlayerControl *pc = new PlayerControl();
	pc->init();
	pc->autorelease();
	pc->_level = level;
	if (level->isBossLevel())
		pc->addBossControls();
	else
		pc->addLevelControls();

	pc->scheduleUpdate();
	return pc;
}

void PlayerControl::addLevelControls()
{
	float screenWidth = Director::getInstance()->getVisibleSize().width;

	_stay = MenuItemImage::create("buttons/left.png","buttons/left.png");
	_slide = MenuItemImage::create("buttons/right.png","buttons/right.png");
	MenuItemImage* jump = MenuItemImage::create("buttons/jump.png","buttons/jump.png", CC_CALLBACK_1(PlayerControl::jumpCallback, this));

	_stay->setScale(0.7);
	_slide->setScale(0.7);
	jump->setScale(0.7);

	_stay->setOpacity(128);
	_slide->setOpacity(128);
	jump->setOpacity(128);

	_stay->setPosition(Vec2(150,150));
	_slide->setPosition(Vec2(screenWidth-300,120));
	jump->setPosition(Vec2(screenWidth-150,300));

	auto levelmenu = Menu::create(_stay,_slide,jump, NULL);
	levelmenu->setPosition(Vec2::ZERO);
	_level->addChild(levelmenu,1);
}

void PlayerControl::addBossControls()
{
	float screenWidth = Director::getInstance()->getVisibleSize().width;

	_left = MenuItemImage::create("buttons/left.png","buttons/left.png");
	_right = MenuItemImage::create("buttons/right.png","buttons/right.png");
	MenuItemImage* jump = MenuItemImage::create("buttons/jump.png","buttons/jump.png", CC_CALLBACK_1(PlayerControl::jumpCallback, this));
	_shoot = MenuItemImage::create("buttons/shoot.png","buttons/shoot.png");

	_left->setScale(0.7);
	_right->setScale(0.7);
	jump->setScale(0.7);
	_shoot->setScale(0.7);

	_left->setOpacity(128);
	_right->setOpacity(128);
	jump->setOpacity(128);
	_shoot->setOpacity(128);

	_left->setPosition(Vec2(150,300));
	_right->setPosition(Vec2(300,120));
	jump->setPosition(Vec2(screenWidth-300,120));
	_shoot->setPosition(Vec2(screenWidth-150,300));

	auto levelmenu = Menu::create(_left,_right,jump, NULL);
	levelmenu->setPosition(Vec2::ZERO);
	_level->addChild(levelmenu,1);
	auto shootbutton = Menu::create(_shoot, NULL);
	shootbutton->setPosition(Vec2::ZERO);
	_level->addChild(shootbutton,1);
}

void PlayerControl::update(float dt)
{
	_counterForShoot += dt;
	//CCLOG("%f",dt);
	CCLOG("%f", this->_counterForShoot);
	if(_counterForShoot >= 1.0)
		{
			_counterForShoot  = 0;
		}

	if (_level->isBossLevel())
	{
		if (_left->isSelected())
			_level->playerBoss->moveLeft();
		if (_right->isSelected())
			_level->playerBoss->moveRight();
		if (_shoot->isSelected())
			_level->playerBoss->shoot(_counterForShoot);
	} else {

		_level->player->run(!_stay->isSelected());
		_level->player->slide(_slide->isSelected());
	}
}

void PlayerControl::jumpCallback(Ref* pSender)
{
	if (_level->isBossLevel())
		_level->playerBoss->jump();
	else
		_level->player->jump();
}

