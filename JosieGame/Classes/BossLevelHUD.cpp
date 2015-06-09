#include "BossLevelHUD.h"

using namespace cocos2d;

BossLevelHUD::BossLevelHUD() {
	_boss_health = 0;
	_boss_health_max = 0;
	_player_health = 3;
	_player_shields = 0;
	_healthbar_boss = nullptr;
	_healthbar_player = nullptr;
}

BossLevelHUD::~BossLevelHUD() {
	CCLOG("~BossLevelHUD");
}

BossLevelHUD* BossLevelHUD::initWithBossHealth(float health)
{
	BossLevelHUD *hud = new BossLevelHUD();
	hud->autorelease();

	hud->_boss_health_max = health;
	hud->_boss_health = health;

	UserDefault *ud = UserDefault::getInstance();
	hud->_player_shields = ud->getIntegerForKey("josie_perk_shied");

	if (ud->getBoolForKey("josie_perk_extralife"))
		hud->_player_health++;


	hud->initHUD();

	return hud;
}

void BossLevelHUD::initHUD()
{
	Sprite *healthbar_frame = Sprite::create("boss_sprites/healthbar_frame.png");
	healthbar_frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	healthbar_frame->setPosition(1920/2, 1080);
	healthbar_frame->setScale(0.7);

	_healthbar_boss = ProgressTimer::create(Sprite::create("boss_sprites/healthbar.png"));
	_healthbar_boss->setType(ProgressTimer::Type::BAR);
	_healthbar_boss->setPercentage(100.0);
	_healthbar_boss->setBarChangeRate(Vec2(1,0));
	_healthbar_boss->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_healthbar_boss->setPosition(1920/2, 1080);
	_healthbar_boss->setScale(0.7);

	Sprite *playerbar_frame = Sprite::create("boss_sprites/playerbar_frame.png");
	playerbar_frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	playerbar_frame->setPosition(1920/2, 0);

	_healthbar_player = ProgressTimer::create(Sprite::create("boss_sprites/playerbar_hearts.png"));
	_healthbar_player->setType(ProgressTimer::Type::BAR);
	_healthbar_player->setBarChangeRate(Vec2(1,0));
	_healthbar_player->setMidpoint(Vec2(0,1));
	_healthbar_player->setPosition(1920/2, 40);
	_healthbar_player->setPercentage(100 * _player_health / 4.0);

	Sprite *playerbar_shield = Sprite::create("boss_sprites/playerbar_shield.png");
	playerbar_shield->setPosition(1203, 40);
	playerbar_shield->setName("sprite_shield");

	Label *lshield = Label::createWithTTF("", "fonts/Marker Felt.ttf", 40);
	lshield->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	lshield->setPosition(1203+30, 40+7);
	lshield->setColor(Color3B(142,142,185));
	lshield->setName("txt_shield_more");

	this->addChild(healthbar_frame);
	this->addChild(_healthbar_boss);
	this->addChild(playerbar_frame);
	this->addChild(_healthbar_player);
	this->addChild(playerbar_shield);
	this->addChild(lshield);

	updateShields();
}

bool BossLevelHUD::reduceBossHealth(float dmg)
{
	_boss_health -= dmg;
	if (_boss_health <= 0) _boss_health = 0;
	_healthbar_boss->setPercentage(100 * _boss_health / _boss_health_max);
	return (_boss_health > 0);
}

bool BossLevelHUD::reducePlayerHealth()
{
	if (_player_shields > 0) {
		_player_shields--;
		UserDefault::getInstance()->setIntegerForKey("josie_perk_shied", _player_shields);
		updateShields();
		return true;
	}

	if (_player_health > 0) _player_health--;
	_healthbar_player->setPercentage(100 * _player_health / 4.0);
	return (_player_health > 0);
}

void BossLevelHUD::updateShields()
{
	std::ostringstream str_shields;

	if (_player_shields==0)
		this->getChildByName("sprite_shield")->setOpacity(0);
	else if (_player_shields > 1)
		str_shields << "+" << _player_shields-1;

	((Label*)this->getChildByName("txt_shield_more"))->setString(str_shields.str());
}
