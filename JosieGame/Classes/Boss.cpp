/*
 * Boss.cpp
 *
 *  Created on: 25.04.2015
 *      Author: danielmugge
 */

#include "Boss.h"
#include "Level.h"
//
#include "PlayerBoss.h"
#include "Projectile.h"
//#include "CCProgressTimer.h"

USING_NS_CC;


Boss::Boss() {
	this->_attackTimer = 7;
	this->_isAlive = true;
	this->_health = 0;
	this->_level = NULL;
	this->left = NULL;
	this->right = NULL;
	this->_playerBoss = NULL;
	this->_healthbar = NULL;
}

Boss::~Boss() {
	// TODO Auto-generated destructor stub
}

Boss* Boss::initWithLevel(Level* level, PlayerBoss* playerboss)
{
	Boss *boss = new Boss();
	if(boss->initWithFile("boss_sprites/boss1.0.png"))
	{

		boss->autorelease();
		boss->setAnchorPoint(Vec2(0.5,1));
		boss->setPosition(960,1080);
		boss->_level = level;
		//
		boss->_playerBoss = playerboss;
		boss->loadWeapons();
		boss->scheduleUpdate();

		boss->setHealth(30.0f);
		boss->_isAlive=true;

		boss->_healthbar = ProgressTimer::create(Sprite::create("boss_sprites/healthbar.png"));
		boss->_healthbar->setType(ProgressTimer::Type::BAR);
		boss->_healthbar->setPercentage(100.0);
		boss->_healthbar->setBarChangeRate(Vec2(1,0));
		boss->_healthbar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		boss->_healthbar->setPosition(960,256);
		boss->addChild(boss->_healthbar,50);
	}

	return boss;
}



void Boss::update(float dt)
{
	int att = (arc4random() % 4) + 1;
	_attackTimer -= dt;
	if(_attackTimer <= 0)
	{
		useAttack(att);
		_attackTimer = 7;
	}

	checkPlayerHit(left, _playerBoss);
	checkPlayerHit(right, _playerBoss);
	checkBossHit();



}

void Boss::loadWeapons()
{
	left = Sprite::create("boss_sprites/tree_hand_left.png");
	right = Sprite::create("boss_sprites/tree_hand_right.png");
	left->setPosition(Vec2(400,600));
	left->setScale(1.7);
	right->setPosition(Vec2(1520,600));
	right->setScale(1.7);
	_level->addChild(left,1);
	_level->addChild(right,1);
}

void Boss::checkPlayerHit(Sprite* weapon, PlayerBoss* target)
{
	if(weapon->getBoundingBox().intersectsRect(target->getBoundingBox()))
	{
		CCLOG("Player was hit");
	}
}

void Boss::checkBossHit(){
	for(Projectile* pr: this->_playerBoss->projectiles)
	{
		if(this->_healthbar->getPercentage() == 0)
			this->_isAlive = false;
		if(pr->checkCollision(this->left) || pr->checkCollision(this->right)){
			this->_healthbar->setPercentage(_healthbar->getPercentage() - 10);
			//_playerBoss->projectiles.eraseObject(pr);
			return;
		}
	}
}


void Boss::useAttack(int attackID){
	switch(attackID){
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

void Boss::setHealth(float health)
{
	_health = health;
}

void Boss::reduceHealth(float dmg)
{
	float afterBeingHit = _health-dmg;

	if(afterBeingHit <= 0.0f)
	{
		_isAlive = false;
	} else
	{
		Boss::setHealth(afterBeingHit);
	}

}


