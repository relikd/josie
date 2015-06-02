#ifndef SHOPSCENE_H_
#define SHOPSCENE_H_

#include "cocos2d.h"

class ShopScene : public cocos2d::Scene {
public:
	ShopScene();
	virtual ~ShopScene();
	static ShopScene* initShop();

	void fight();

private:
	cocos2d::Layer *_equippedLayer;
	cocos2d::Menu *_menu;

	int priceForColumn(int column);
	void createLabels();
	void createButtons();
	void upgrade(Ref* p);

	int perkGetValue(int row, int column);
	void perkSetValue(int row, int column, int newValue);
	void perkUp(int row, int column);
	void spendMoney(int coins);

	void perkDisable(cocos2d::MenuItemImage *btn);
	void perkEnable(cocos2d::MenuItemImage *btn);
	void perkEquip(cocos2d::MenuItemImage *btn);
	void updateButtonState();
};

#endif /* SHOPSCENE_H_ */
