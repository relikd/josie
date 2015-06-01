#ifndef BOSSLEVEL_H_
#define BOSSLEVEL_H_

class Projectile;
class PlayerBoss;

#include "cocos2d.h"

class BossLevel : public cocos2d::Scene
{
public:
	BossLevel();
	virtual ~BossLevel();
	static BossLevel* initWithOptions();

	float getHealth() { return _health;}
	bool isAlive() { return (_health>0); }

	cocos2d::Vector<Projectile*> projectiles;

private:
	void createUI();
	void loadWeapons();
	void update(float dt);
	void reduceHealth(float dmg);
	void useAttack(int attackID);
	void checkPlayerHit(cocos2d::Sprite* weapon, PlayerBoss* target);
	void checkBossHit();
	void finishAnimation(bool won);

	float _health;
	float _health_max;
	cocos2d::ProgressTimer* _healthbar;
	cocos2d::Sprite* left;
	cocos2d::Sprite* right;
	float _attackTimer;
	PlayerBoss* _playerBoss;
};

#endif /* BOSSLEVEL_H_ */
