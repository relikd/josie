#include "BossLevel.h"
#include "Projectile.h"
#include "PlayerBoss.h"
#include "PlayerControl.h"

using namespace cocos2d;

BossLevel::BossLevel() {
	_health = 0;
	_health_max = 0;
	_healthbar = NULL;
	left = NULL;
	right = NULL;
	_attackTimer = 0;
	_playerBoss = NULL;
}

BossLevel::~BossLevel() {
	CCLOG("~BossLevel");
}

BossLevel* BossLevel::initWithOptions()
{
	BossLevel *boss = new BossLevel();
	boss->autorelease();
	boss->_health_max = 10;
	boss->_health = boss->_health_max;

	boss->createUI();
	boss->loadWeapons();
	boss->projectiles = Vector<Projectile*>{200};

	boss->scheduleUpdate();

	return boss;
}

void BossLevel::createUI()
{
	// TODO: durch static image ersetzen
	TMXTiledMap *map = TMXTiledMap::create("tilemaps/1.0.tmx");

	// Background Image
	Sprite *background = Sprite::create("backgrounds/bg_1.0.png");
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	// Baumkrone / Gegner
	Sprite *enemy = Sprite::create("boss_sprites/boss1.0.png");
	enemy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	enemy->setPosition(1920/2, 1080);

	// load Player
	_playerBoss = PlayerBoss::initWithLevel(this);
	_playerBoss->setPosition(Vec2(200, 60));
	_playerBoss->setScale(0.7);

	// Player Controls
	PlayerControl *playerControl = PlayerControl::initWithBossPlayer(_playerBoss);

	// Lebensanzeige
	Sprite *healthbar_frame = Sprite::create("boss_sprites/healthbar_frame.png");
	healthbar_frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	healthbar_frame->setPosition(1920/2, 1080);
	healthbar_frame->setScale(0.7);

	_healthbar = ProgressTimer::create(Sprite::create("boss_sprites/healthbar.png"));
	_healthbar->setType(ProgressTimer::Type::BAR);
	_healthbar->setPercentage(100.0);
	_healthbar->setBarChangeRate(Vec2(1,0));
	_healthbar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_healthbar->setPosition(1920/2, 1080);
	_healthbar->setScale(0.7);

	this->addChild(background);
	this->addChild(enemy);
	this->addChild(map);
	this->addChild(_playerBoss);
	this->addChild(playerControl);
	this->addChild(healthbar_frame);
	this->addChild(_healthbar);
}

void BossLevel::loadWeapons()
{
	left = Sprite::create();
	right = Sprite::create();

	left->setContentSize(Size(185,150));
	right->setContentSize(Size(185,150));
	left->setPosition(Vec2(400,600));
	right->setPosition(Vec2(1520,600));
	left->setScale(1.7);
	right->setScale(1.7);

	Sprite *l = Sprite::create("boss_sprites/tree_hand_left.png");
	Sprite *r = Sprite::create("boss_sprites/tree_hand_right.png");
	l->setAnchorPoint(Vec2::ZERO);
	r->setAnchorPoint(Vec2::ZERO);
	l->setPosition(-66, 8);
	r->setPosition(-5, 8);
	left->addChild(l);
	right->addChild(r);

	this->addChild(left, 1);
	this->addChild(right, 1);
}

void BossLevel::update(float dt)
{
	_attackTimer -= dt;
	if(_attackTimer < 0) {
		int att = (arc4random() % 4) + 1;
		useAttack(att);
		_attackTimer = 7; // every 7 seconds an attack
	}

	checkPlayerHit(left, _playerBoss);
	checkPlayerHit(right, _playerBoss);
	checkBossHit();
}

void BossLevel::checkPlayerHit(Sprite* weapon, PlayerBoss* target)
{
	if(weapon->getBoundingBox().intersectsRect(target->getBoundingBox()))
	{
		//CCLOG("Player was hit");
	}
}

void BossLevel::checkBossHit()
{
	for(Projectile* pr: this->projectiles)
	{
		if(pr->hasCollision(this->left) || pr->hasCollision(this->right)){
			int projectile_damage = UserDefault::getInstance()->getIntegerForKey("josie_perk_damage");
			reduceHealth(projectile_damage);
			return;
		}
	}
}

void BossLevel::reduceHealth(float dmg)
{
	_health -= dmg;
	if (_health <= 0) {
		_health = 0;
		finishAnimation(true);
	}
	_healthbar->setPercentage(100 * _health / _health_max);
}

void BossLevel::finishAnimation(bool won)
{
	// TODO: some crazy fanzy animation
	Director::getInstance()->popScene();
}


void BossLevel::useAttack(int attackID)
{
	switch(attackID) {
	case 1:
	{
		auto left_rotate = RotateTo::create(1.0,90.0f);
		auto right_rotate = RotateTo::create(1.0, 270.0f);
		auto left_rotate_back = RotateTo::create(0.2,0.0f);
		auto right_rotate_back = RotateTo::create(0.2, 0.0f);
		auto left_down = MoveTo::create(0.2,Vec2(400,100));
		auto left_up = MoveTo::create(1.0,Vec2(400,600));
		auto right_down = MoveTo::create(0.2,Vec2(1520,100));
		auto right_up = MoveTo::create(1.0,Vec2(1520,600));
		auto sequence_for_left = Sequence::create(left_rotate,left_down,left_up,left_rotate_back, nullptr);
		auto sequence_for_right = Sequence::create(right_rotate,right_down,right_up, right_rotate_back,nullptr);
		left->runAction(sequence_for_left);
		right->runAction(sequence_for_right);
		break;
	}
	case 2:
	{
		auto left_rotate = RotateTo::create(2.0,30.0f);
		auto left_rotate_back = RotateTo::create(0.2,0.0f);
		auto left_down = MoveTo::create(0.2,Vec2(1520,100));
		auto left_up = MoveTo::create(1.0,Vec2(400,600));
		auto sequence  = Sequence::create(left_rotate, left_down, left_up, left_rotate_back, nullptr);
		left->runAction(sequence);
		break;
	}
	case 3:
	{
		auto right_rotate = RotateTo::create(2.0, 30.0f);
		auto right_rotate_back = RotateTo::create(0.2, 0.0f);
		auto right_down = MoveTo::create(0.2,Vec2(400,100));
		auto right_up = MoveTo::create(1.0,Vec2(1520,600));
		auto sequence = Sequence::create(right_rotate, right_down, right_up,right_rotate_back, nullptr);
		right->runAction(sequence);
		break;
	}
	case 4:
	{
		auto left_to_mid = MoveTo::create(1.0,Vec2(832,600));
		auto right_to_mid = MoveTo::create(1.0, Vec2(1088,600));
		auto mid_to_left = MoveTo::create(1.0,Vec2(400,600));
		auto mid_to_right = MoveTo::create(1.0, Vec2(1520,600));
		auto left_down = MoveTo::create(0.2,Vec2(832,100));
		auto left_up = MoveTo::create(1.0,Vec2(832,600));
		auto right_down = MoveTo::create(0.2,Vec2(1088,100));
		auto right_up = MoveTo::create(1.0,Vec2(1088,600));
		auto sequence_for_left = Sequence::create(left_to_mid,left_down, left_up,mid_to_left,nullptr);
		auto sequence_for_right = Sequence::create(right_to_mid, right_down, right_up, mid_to_right,nullptr);
		left->runAction(sequence_for_left);
		right->runAction(sequence_for_right);
		break;
	}
	}
}

