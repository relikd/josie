/*
 * Projectile.h
 *
 *  Created on: 27.04.2015
 *      Author: danielmugge
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "cocos2d.h"


class Projectile : public cocos2d::Sprite
{
public:
	Projectile();
	virtual ~Projectile();

	static Projectile* init(float x, float y);

	void killProjectile();
	bool isAlive() { return _isAlive;} ;
	float getXPos() {return _xPos ; } ;
	float getYPos() {return _yPos ; } ;


private:
	bool _isAlive;
	float _xPos;
	float _yPos;

};



#endif /* PROJECTILE_H_ */
