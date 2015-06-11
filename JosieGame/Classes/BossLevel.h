#ifndef BOSSLEVEL_H_
#define BOSSLEVEL_H_

class Projectile;
class BossPlayer;
class CollisionLayer;
class BossLevelHUD;

#include "cocos2d.h"

class BossLevel : public cocos2d::Scene
{
public:
	BossLevel();
	virtual ~BossLevel();
	static BossLevel* initWithOptions();

	cocos2d::Vector<Projectile*> projectiles;

private:
	void createUI();
	void loadWeapons();
	void update(float dt);

	void checkPlayerHit(CollisionLayer* weapon);
	void checkBossHit();

	void battleEndedWon(bool won);
	void bossAttack();

	BossLevelHUD* _hud;
	BossPlayer* _playerBoss;
	CollisionLayer* left;
	CollisionLayer* right;
	float _attackTimer;
	float _timeSinceLastHit;
};

#endif /* BOSSLEVEL_H_ */
