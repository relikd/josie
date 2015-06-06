#ifndef BOSSLEVEL_H_
#define BOSSLEVEL_H_

class Projectile;
class PlayerBoss;
class CollisionLayer;

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

	void checkPlayerHit(CollisionLayer* weapon);
	void checkBossHit();

	void reduceBossHealth(float dmg);
	void reducePlayerHealth();

	void battleEndedWon(bool won);
	void bossAttack();

	float _health;
	float _health_max;
	int _playerHealth;
	cocos2d::ProgressTimer* _healthbar;
	CollisionLayer* left;
	CollisionLayer* right;
	PlayerBoss* _playerBoss;
	float _attackTimer;
	float _timeSinceLastHit;
};

#endif /* BOSSLEVEL_H_ */
