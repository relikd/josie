#ifndef SHOPSCENE_H_
#define SHOPSCENE_H_

#include "cocos2d.h"

class ShopScene : public cocos2d::Scene {
public:
	ShopScene();
	virtual ~ShopScene();
	void createButtons();

	static ShopScene* initShop();

	void perkDisable(cocos2d::MenuItemImage *btn);
	void perkEnable(cocos2d::MenuItemImage *btn);
	void perkEquip(cocos2d::MenuItemImage *btn);

	int priceForColumn(int column);
	void updateDisabledButtons();

	void upgrade(Ref* p);

private:
	cocos2d::Layer *_equippedLayer;
	cocos2d::Menu *_menu;
};

#endif /* SHOPSCENE_H_ */
