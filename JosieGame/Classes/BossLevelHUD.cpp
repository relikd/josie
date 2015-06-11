#include "BossLevelHUD.h"

#include "BossPlayer.h"

using namespace cocos2d;

BossLevelHUD::BossLevelHUD() {
	_boss_health = 0;
	_boss_health_max = 0;
	_player_health = 3;
	_healthbar_boss = nullptr;
	_healthbar_player = nullptr;

	// Player Controls
	_left = nullptr;
	_right = nullptr;
	_shoot = nullptr;
	_bossplayer = nullptr;
	_timeSinceLastShot = 0;
}

BossLevelHUD::~BossLevelHUD() {
	CCLOG("~BossLevelHUD");
}

BossLevelHUD* BossLevelHUD::initWithBossHealth(float health, BossPlayer* p)
{
	BossLevelHUD *hud = new BossLevelHUD();
	hud->autorelease();

	hud->_bossplayer = p;
	hud->_boss_health_max = health;
	hud->_boss_health = health;

	if (UserDefault::getInstance()->getBoolForKey("josie_perk_extralife"))
		hud->_player_health++;

	hud->addBossHealthbar();
	hud->addPlayerHealthbar();
	hud->addPlayerControls();

	hud->scheduleUpdate();

	return hud;
}


//
// Create UI
//

void BossLevelHUD::addBossHealthbar()
{
	Sprite *healthbar_frame = Sprite::create("boss_sprites/healthbar_frame.png");
	healthbar_frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	healthbar_frame->setPosition(1920 / 2, 1080);
	healthbar_frame->setScale(0.7);

	_healthbar_boss = ProgressTimer::create(Sprite::create("boss_sprites/healthbar.png"));
	_healthbar_boss->setType(ProgressTimer::Type::BAR);
	_healthbar_boss->setPercentage(100.0);
	_healthbar_boss->setBarChangeRate(Vec2(1, 0));
	_healthbar_boss->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_healthbar_boss->setPosition(1920 / 2, 1080);
	_healthbar_boss->setScale(0.7);

	this->addChild(healthbar_frame);
	this->addChild(_healthbar_boss);
}

void BossLevelHUD::addPlayerHealthbar()
{
	Sprite *playerbar_frame = Sprite::create("boss_sprites/playerbar_frame.png");
	playerbar_frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	playerbar_frame->setPosition(1920/2, 0);

	_healthbar_player = ProgressTimer::create(Sprite::create("boss_sprites/playerbar_hearts.png"));
	_healthbar_player->setType(ProgressTimer::Type::BAR);
	_healthbar_player->setBarChangeRate(Vec2(1, 0));
	_healthbar_player->setMidpoint(Vec2(0, 1));
	_healthbar_player->setPosition(1920/2, 40);
	_healthbar_player->setPercentage(100 * _player_health / 4.0);

	Sprite *shield = Sprite::create("boss_sprites/playerbar_shield.png");
	shield->setPosition(1203, 40);
	shield->setName("sprite_shield");

	Label *lshield = Label::createWithTTF("", "fonts/Marker Felt.ttf", 40);
	lshield->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	lshield->setPosition(1203 + 30, 40 + 7);
	lshield->setColor(Color3B(142, 142, 185));
	lshield->setName("txt_shield_more");

	this->addChild(playerbar_frame);
	this->addChild(_healthbar_player);
	this->addChild(shield);
	this->addChild(lshield);

	updateShields();
}

void BossLevelHUD::addPlayerControls()
{
	_left = MenuItemImage::create("buttons/left.png","buttons/left.png");
	_right = MenuItemImage::create("buttons/right.png","buttons/right.png");
	_shoot = MenuItemImage::create("buttons/shoot.png","buttons/shoot.png");

	_left->setScale(0.7);
	_right->setScale(0.7);
	_shoot->setScale(0.7);

	_left->setOpacity(128);
	_right->setOpacity(128);
	_shoot->setOpacity(128);

	_left->setPosition(Vec2(150,300));
	_right->setPosition(Vec2(300,120));
	_shoot->setPosition(Vec2(1920-150,120));

	Menu *menu = Menu::create(_left,_right, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	Menu *shootbutton = Menu::create(_shoot, NULL);
	shootbutton->setPosition(Vec2::ZERO);
	this->addChild(shootbutton);
}


//
// Update UI
//

void BossLevelHUD::updateShields()
{
	int shields = UserDefault::getInstance()->getIntegerForKey("josie_perk_shied");
	std::ostringstream str_shields;

	if (shields <= 0)
		this->getChildByName("sprite_shield")->setOpacity(0);
	if (shields > 1)
		str_shields << "+" << shields-1;

	((Label*)this->getChildByName("txt_shield_more"))->setString(str_shields.str());
}


//
// Gameplay Functionality
//

bool BossLevelHUD::reduceBossHealth(float dmg)
{
	_boss_health -= dmg;
	if (_boss_health <= 0) _boss_health = 0;
	_healthbar_boss->setPercentage(100 * _boss_health / _boss_health_max);
	return (_boss_health > 0);
}

bool BossLevelHUD::reducePlayerHealth()
{
	UserDefault *ud = UserDefault::getInstance();
	int shields = ud->getIntegerForKey("josie_perk_shied");

	if (shields > 0) {
		ud->setIntegerForKey("josie_perk_shied", shields-1);
		updateShields();
		return true;
	}

	if (_player_health > 0) _player_health--;
	_healthbar_player->setPercentage(100 * _player_health / 4.0);
	return (_player_health > 0);
}


//
// Other Functionality
//

void BossLevelHUD::update(float dt)
{
	_timeSinceLastShot += dt;
	if (_left->isSelected())
		_bossplayer->moveLeft();
	if (_right->isSelected())
		_bossplayer->moveRight();
	if (_shoot->isSelected()) {
		if (_bossplayer->shoot(_timeSinceLastShot)) {
			_timeSinceLastShot = 0;
		}
	}
}

