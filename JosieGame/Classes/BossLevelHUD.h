#ifndef BOSSLEVELHUD_H_
#define BOSSLEVELHUD_H_

#include "cocos2d.h"

class BossLevelHUD : public cocos2d::Layer
{
public:
	BossLevelHUD();
	virtual ~BossLevelHUD();
	static BossLevelHUD* initWithBossHealth(float health);
	void initHUD();

	bool reduceBossHealth(float dmg); // returns false if killed
	bool reducePlayerHealth(); // returns false if died
private:
	float _boss_health;
	float _boss_health_max;
	int _player_health;
	int _player_shields;
	cocos2d::ProgressTimer* _healthbar_boss;
	cocos2d::ProgressTimer* _healthbar_player;

	void updateShields();
};

#endif /* BOSSLEVELHUD_H_ */
