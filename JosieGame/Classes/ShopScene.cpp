#include "ShopScene.h"

#define TAG_OFFSET 333

using namespace cocos2d;
// temporary
int credits = 60;
int dmg=1;
int shot=1;
int freq=1;
int speed=1;
bool heart = 0;
bool shield = 0;

ShopScene::ShopScene() {
	_equippedLayer = NULL;
	_menu = NULL;
	_txtCredits = NULL;
}

ShopScene::~ShopScene() {}

ShopScene* ShopScene::initShop()
{
	ShopScene *scene = new ShopScene();
    scene->_equippedLayer = Layer::create();
    scene->_menu = Menu::create();

    Sprite *bg = Sprite::create("backgrounds/bg_shop.png");
    bg->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);

    scene->addChild(bg);
    scene->addChild(scene->_menu);
    scene->addChild(scene->_equippedLayer);

    scene->createLabels();
    scene->createButtons();
    scene->_menu->setPosition(Vec2::ZERO);

    return scene;
}

void ShopScene::createLabels() {
	_txtCredits = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 90);
	_txtCredits->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	_txtCredits->setPosition(1740,1050); // 30px padding
	_txtCredits->setColor(Color3B(165,42,42));

	Label *l1 = Label::createWithTTF("10", "fonts/Marker Felt.ttf", 60);
	Label *l2 = Label::createWithTTF("20", "fonts/Marker Felt.ttf", 60);
	Label *l3 = Label::createWithTTF("40", "fonts/Marker Felt.ttf", 60);
	l1->setPosition(180+200+30, 1000);
	l2->setPosition(l1->getPositionX()+200+30,l1->getPositionY());
	l3->setPosition(l2->getPositionX()+200+30,l2->getPositionY());

	this->addChild(_txtCredits);
	this->addChild(l1);
	this->addChild(l2);
	this->addChild(l3);
}

void ShopScene::createButtons()
{
	float btn_size = 200;
	float btn_padding = 30;
	float x1 = 165;
	float y1 = 165;

	//Menu *m = Menu::create();
	for (int row=0; row<4; row++)
	{
		std::ostringstream btnstr;
		btnstr << "buttons/shopbuttons/";
		switch (row) {
			case 3: btnstr << "dmg"; break;
			case 2: btnstr << "shot"; break;
			case 1: btnstr << "frequency"; break;
			case 0: btnstr << "special"; break;
		}

		for (int column=0; column<4; column++)
		{
			std::ostringstream btnfullstr;
			btnfullstr << btnstr.str() << column+1 << ".png";

			MenuItemImage *mii = MenuItemImage::create(btnfullstr.str(), btnfullstr.str(), CC_CALLBACK_1(ShopScene::upgrade, this));
			mii->setPosition(x1 + column*(btn_size + btn_padding), y1 + row*(btn_size + btn_padding));
			mii->setTag((row<<2) + column + TAG_OFFSET);
			_menu->addChild(mii, 2);
		}
	}

	updateButtonState();
}

void ShopScene::updateButtonState()
{
	std::ostringstream crstr;
	crstr << credits;
	_txtCredits->setString(crstr.str());

	_equippedLayer->removeAllChildren();

	for (int i = 0; i<16; i++)
	{
		MenuItemImage *im = (MenuItemImage*)_menu->getChildByTag(i + TAG_OFFSET);
		int column = i&3;
		int row = i>>2;

		int perkVal = 0;
		if (row == 3) perkVal = dmg;
		else if (row == 2) perkVal = shot;
		else if (row == 1) perkVal = freq;
		else perkVal = speed;

		perkDisable(im);

		if (perkVal > column) {
			perkEquip(im);
		} else if (perkVal == column && credits >= priceForColumn(column)) {
			perkEnable(im);
		}
	}

	// extra processing for extra life and shield
	MenuItemImage *mshield = (MenuItemImage*)_menu->getChildByTag(2 + TAG_OFFSET);
	MenuItemImage *mextralife = (MenuItemImage*)_menu->getChildByTag(3 + TAG_OFFSET);
	if (shield) perkEquip(mshield);
	if (heart) perkEquip(mextralife);
	if (credits >= priceForColumn(2)) {
		perkEnable(mshield);
		if (credits >= priceForColumn(3))
			perkEnable(mextralife);
	}
}

int ShopScene::priceForColumn(int column)
{
	switch (column) {
		case 0: return 0;
		case 1: return 10;
		case 2: return 20;
		case 3: return 40;
	}
}

void ShopScene::upgrade(Ref* p)
{
	int tag = ((MenuItemImage*)p)->getTag()-TAG_OFFSET;
	int row = tag>>2;
	int column = tag&3;

	if (row == 3) dmg++;
	else if (row == 2) shot++;
	else if (row == 1) freq++;
	else if (column<2) speed++;
	else if (column==2) shield=true;
	else if (column==3) heart=true;

	credits -= priceForColumn(column);

	updateButtonState();
}

void ShopScene::perkDisable(MenuItemImage *btn) {
	btn->setEnabled(false);
	btn->setOpacity(128);
}

void ShopScene::perkEnable(MenuItemImage *btn) {
	btn->setEnabled(true);
	btn->setOpacity(255);;
}

void ShopScene::perkEquip(MenuItemImage *btn) {
	btn->setEnabled(false);
	btn->setOpacity(255);

	Sprite *frame = Sprite::create("buttons/shopbuttons/purchased.png");
	frame->setPosition(btn->getPosition());
	_equippedLayer->addChild(frame);
}
