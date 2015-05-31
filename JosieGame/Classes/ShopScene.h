#ifndef SHOPSCENE_H_
#define SHOPSCENE_H_

#include "cocos2d.h"

class ShopScene : public cocos2d::Scene {
public:
	ShopScene();
	virtual ~ShopScene();
	static ShopScene* initShop();

	void createLabels();
	void createButtons();
	void updateButtonState();

	int priceForColumn(int column);
	void upgrade(Ref* p);

	void perkDisable(cocos2d::MenuItemImage *btn);
	void perkEnable(cocos2d::MenuItemImage *btn);
	void perkEquip(cocos2d::MenuItemImage *btn);

private:
	cocos2d::Layer *_equippedLayer;
	cocos2d::Menu *_menu;
	cocos2d::Label *_txtCredits;
};

#endif /* SHOPSCENE_H_ */
