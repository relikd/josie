#ifndef SHOPSCENE_H_
#define SHOPSCENE_H_

#include "cocos2d.h"

class ShopScene : public cocos2d::Layer {
public:
	ShopScene();
	virtual ~ShopScene();
	void createButtons();

	static cocos2d::Scene* createScene();

	void perkDisable(cocos2d::MenuItemImage *btn);
	void perkEnable(cocos2d::MenuItemImage *btn);
	void perkEquip(cocos2d::MenuItemImage *btn);

	void upgradeShoot(Ref* p);
	void upgradeDamage(Ref* p);
	void upgradeFrequency(Ref* p);
	void upgradeSpeed(Ref* p);
	void upgradeShield(Ref* p);
	void upgradeHeart(Ref* p);
};

#endif /* SHOPSCENE_H_ */
