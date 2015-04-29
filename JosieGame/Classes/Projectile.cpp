/*
 * Projectile.cpp
 *
 *  Created on: 27.04.2015
 *      Author: danielmugge
 */

#include "Projectile.h"

using namespace cocos2d;

Projectile::Projectile() {

	_isAlive = true;
	CCLOG("projectile is Alive");
	_xPos = 0.0f;
	_yPos = 0.0f;

}

Projectile::~Projectile() {
	CCLOG("projectile destroyed");
	this->removeFromParent();
}

Projectile* Projectile::init(float x , float y)
{
	Projectile* pr = new Projectile();
	pr->initWithFile("particles/std_bullet.png");
	pr->autorelease();
	pr->_xPos = x;
	pr->_yPos = y;


	CCLOG("Projectile has been initialized");
	return pr;
}

void Projectile::killProjectile()
{
	CCLOG("Projectile should be not alive anymore");
	_isAlive = false;
}


