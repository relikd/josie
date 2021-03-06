#include "BossLevel.h"
#include "Projectile.h"
#include "BossLevelHUD.h"
#include "BossPlayer.h"
#include "AudioUnit.h"
#include "LevelSelectScene.h"
#include "GameStateManager.h"

using namespace cocos2d;

#define BOSSPLAYER_IMMORTAL_TIME 2.5
#define BOSS_SLOWEST_ATTACK_TIME 7.0

BossLevel::BossLevel() {
	_hud = nullptr;
	_playerBoss = nullptr;
	left = nullptr;
	right = nullptr;
	_attackTimer = 3;
	_timeSinceLastHit = BOSSPLAYER_IMMORTAL_TIME;
	_difficulty = 1;
	AudioUnit::preloadBossSounds();
}
BossLevel::~BossLevel() {
	AudioUnit::stopBackground();
	AudioUnit::unloadBossSounds();
}

BossLevel* BossLevel::createBossDifficulty(int difficulty)  // 1 - 10
{
	BossLevel *boss = new BossLevel();
	boss->autorelease();

	if (difficulty > 0)
		boss->_difficulty = difficulty;

	boss->createUI();
	boss->loadWeapons();
	boss->projectiles = Vector<Projectile*>{200}; // maybe more if frequency rises

	boss->scheduleUpdate();

	return boss;
}

void BossLevel::createUI()
{
	AudioUnit::startBackgroundBoss();

	// Background Image
	Sprite *background = Sprite::create("backgrounds/bg_1.0.png");
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	//Mimik Images
	Sprite *mimik_normal = Sprite::create("boss_sprites/tree_mimik_normal.png");
	mimik_normal->setPosition(Vec2(1920/2, 1080/6 * 4));
	mimik_normal->setTag(400);
	mimik_normal->setVisible(true);
	this->addChild(mimik_normal,0);


	Sprite *mimik_hurt = Sprite::create("boss_sprites/tree_mimik_hurt.png");
	mimik_hurt->setPosition(Vec2(1920/2, 1080/6 * 4));
	mimik_hurt->setTag(401);
	mimik_hurt->setVisible(false);
	this->addChild(mimik_hurt,0);

	// load Player
	_playerBoss = BossPlayer::createWithLevel(this);
	_playerBoss->setPosition((1920/2), 108);

	_hud = BossLevelHUD::initWithBossHealth( 10 + 3 * pow(_difficulty, 3) );

	this->addChild(background,-1);
	this->addChild(_playerBoss,1);
	this->addChild(_hud,2);
}

void BossLevel::loadWeapons()
{
	left = CollisionLayer::createWithSize(180,140);
	right = CollisionLayer::createWithSize(180,140);
	left->collisionType = CollisionLayerTypeBossEnemy;
	right->collisionType = CollisionLayerTypeBossEnemy;
	left->insertImageName("boss_sprites/tree_hand_left.png", Vec2(-66,-15), Vec2::ANCHOR_BOTTOM_LEFT);
	right->insertImageName("boss_sprites/tree_hand_right.png", Vec2(-10,-15), Vec2::ANCHOR_BOTTOM_LEFT);

	left->setPosition(Vec2(400,600));
	right->setPosition(Vec2(1520,600));
	left->setScale(1.7);
	right->setScale(1.7);

	this->addChild(left, 0);
	this->addChild(right, 0);
}

void BossLevel::update(float dt)
{
	_attackTimer -= dt;
	_timeSinceLastHit += dt;

	if(_attackTimer < 0) {
		bossAttack();
		_attackTimer = BOSS_SLOWEST_ATTACK_TIME / sqrt(_difficulty);
	}

	checkPlayerHit(left);
	checkPlayerHit(right);
	checkBossHit();
}

void BossLevel::checkPlayerHit(CollisionLayer* weapon)
{
	if (_timeSinceLastHit > BOSSPLAYER_IMMORTAL_TIME && weapon->getCollision(_playerBoss))
	{
		AudioUnit::playJosieHitSound();
		_timeSinceLastHit = 0;
		_playerBoss->runAction(Blink::create(BOSSPLAYER_IMMORTAL_TIME, 15));
		if (_hud->reducePlayerHealth() == false) {
			battleEndedWon(false);
		}
	}
}

void BossLevel::checkBossHit()
{
	for(Projectile* pr: this->projectiles)
	{
		if (pr->getCollision(left) || pr->getCollision(right))
		{
			pr->killProjectile(true);
			int projectile_damage = UserDefault::getInstance()->getIntegerForKey("josie_perk_damage");

			//AudioUnit::playProjectileHitSound(); //To much but if you want to you can activate it
			AudioUnit::playBossHitSound();
			getChildByTag(401)->setVisible(true);
			getChildByTag(400)->setVisible(false);
			CallFuncN *hurt = CallFuncN::create(CC_CALLBACK_0(Node::setVisible, getChildByTag(401), false));
			CallFuncN *normal = CallFuncN::create(CC_CALLBACK_0(Node::setVisible, getChildByTag(400), true));
			Sequence* gotHit = Sequence::create(DelayTime::create(0.2), hurt, normal,nullptr);
			this->runAction(gotHit);


			if (_hud->reduceBossHealth(projectile_damage) == false) {
				battleEndedWon(true);
			}
			return;
		}
	}
}


//
// Logical Game Management
//

void BossLevel::battleEndedWon(bool won)
{
	Sequence *tintFade = Sequence::createWithTwoActions(TintTo::create(0.7, 255, 0, 0), FadeTo::create(0.3, 0));
	DelayTime *delay = DelayTime::create(tintFade->getDuration()+0.5);
	CallFuncN *popToMenu = CallFuncN::create(CC_CALLBACK_0(BossLevel::backToMenu, this));

	AudioUnit::stopBackground();
	AudioUnit::startBackgroundLevelSelect();

	this->unscheduleUpdate();
	this->runAction(Sequence::createWithTwoActions(delay, popToMenu));
	
	if (won) {
		GameStateManager::setBossDefeated(_difficulty);
		UserDefault::getInstance()->setIntegerForKey("boss_difficulty", UserDefault::getInstance()->getIntegerForKey("boss_difficulty") +1);
		
		left->stopAllActions();
		right->stopAllActions();
		left->spriteImage->runAction(tintFade->clone());
		right->spriteImage->runAction(tintFade);
	} else {
		_playerBoss->spriteImage->runAction(tintFade);
	}
}

void BossLevel::backToMenu()
{
	Director::getInstance()->replaceScene(LevelSelect::createSceneWithLevel(1));
}

void BossLevel::bossAttack()
{

	int attackID = (arc4random() % 4) + 1;
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

