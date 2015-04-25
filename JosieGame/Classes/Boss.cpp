/*
 * Boss.cpp
 *
 *  Created on: 25.04.2015
 *      Author: danielmugge
 */

#include "Boss.h"

USING_NS_CC;

float _health;
bool _isAlive;



Boss::Boss() {
	this->_isAlive = true;
	this->_health = 0;
	this->_level = NULL;

}

Boss::~Boss() {
	// TODO Auto-generated destructor stub
}

Boss* Boss::initWithLevel(Level* level)
{
	Boss *boss = new Boss();
	if(boss->initWithFile("boss/boss1.0.png"))
	{
		boss->autorelease();
		boss->setAnchorPoint(Vec2(0.5,1));
		boss->_level = level;

		setHealth(30.0f);
		_isAlive=true;
	}

	return boss;
}

void Boss::useAttack(){

}

void Boss::setHealth(float health)
{
	_health = health;
}

void Boss::reduceHealth(float dmg)
{
	afterBeingHit = _health-dmg;

	if(afterBeingHit <= 0.0f)
	{
		_isAlive = false;
	} else
	{
		Boss::setHealth(afterBeingHit);
	}

}


