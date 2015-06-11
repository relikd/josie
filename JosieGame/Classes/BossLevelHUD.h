#ifndef BOSSLEVELHUD_H_
#define BOSSLEVELHUD_H_

#include "cocos2d.h"

class PlayerBoss;

class BossLevelHUD : public cocos2d::Layer
{
public:
	BossLevelHUD();
	virtual ~BossLevelHUD();
	static BossLevelHUD* initWithBossHealth(float health, PlayerBoss* p);

	bool reduceBossHealth(float dmg); // returns false if killed
	bool reducePlayerHealth(); // returns false if died
private:
	float _boss_health;
	float _boss_health_max;
	int _player_health;
	cocos2d::ProgressTimer* _healthbar_boss;
	cocos2d::ProgressTimer* _healthbar_player;

	void initHUD();
	void addBossHealthbar();
	void addPlayerHealthbar();
	void addPlayerControls();

	void updateShields();
	void update(float dt);

	// Player Controls
	cocos2d::MenuItemImage *_left;
	cocos2d::MenuItemImage *_right;
	cocos2d::MenuItemImage *_shoot;
	PlayerBoss* _bossplayer;
	float _timeSinceLastShot;
};

#endif /* BOSSLEVELHUD_H_ */
