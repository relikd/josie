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
class PlayerBoss;

class Boss : public cocos2d::Sprite
{
public:
	Boss();
	virtual ~Boss();
	static Boss* initWithLevel(Level* level,PlayerBoss* playerboss);


	void update(float dt);
	void useAttack(int attackID);
	void reduceHealth(float dmg);
	void setHealth(float health);
	void loadWeapons();
	void checkPlayerHit(Sprite* weapon, PlayerBoss* target);
	void checkBossHit();

	float getHealth() { return _health ;}
	bool isAlive() { return _isAlive ; }


private:
	float _attackTimer;
	float _health;
	bool _isAlive;
	Level* _level;
	PlayerBoss* _playerBoss;
	Sprite* left;
	Sprite* right;
	cocos2d::ProgressTimer* _healthbar;


};



#endif /* BOSS_H_ */
