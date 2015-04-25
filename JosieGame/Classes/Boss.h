/*
 * Boss.h
 *
 *  Created on: 25.04.2015
 *      Author: danielmugge
 */

#ifndef BOSS_H_
#define BOSS_H_

#include "cocos2d.h"
class Level;

class Boss : public cocos2d::Sprite
{
public:
	Boss();
	virtual ~Boss();
	static Boss* initWithLevel(Level* level);


	void useAttack();
	void reduceHealth(float dmg);
	void setHealth(float health);


	float getHealth() { return _health ;}
	bool isAlive() { return _isAlive ; }


private:
	float _health;
	bool _isAlive;
	Level* _level;


};



#endif /* BOSS_H_ */