#include "ShopScene.h"
#include "BossLevel.h"
#include "GameStateManager.h"
#include "BossPlayer.h"
#include "AudioUnit.h"

#define TAG_OFFSET 333
#define BUTTON_SIZE 200
#define BUTTON_PADDING 30

using namespace cocos2d;

ShopScene::ShopScene() {
	_equippedLayer = NULL;
	_shopWindow = nullptr;
	_menu = NULL;
}
ShopScene::~ShopScene() {}

ShopScene* ShopScene::initShop()
{
	ShopScene *scene = new ShopScene();
	scene->autorelease();
    scene->_equippedLayer = Layer::create();
    scene->_menu = Menu::create();
    scene->_menu->setPosition(Vec2::ZERO);

    scene->_shopWindow = Layer::create();
    scene->_shopWindow->setPosition(Vec2::ZERO);
	
	LayerColor* brownScreen = LayerColor::create(Color4B(201,158,103,255));
	scene->addChild(brownScreen);
	
    Sprite *bg = Sprite::create("backgrounds/bg_shop.png");
    bg->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);

    scene->addChild(bg);
    scene->addChild(scene->_menu);
    scene->addChild(scene->_equippedLayer);
    scene->addChild(scene->_shopWindow);

    scene->showJosieInShopWindow(2,0);

    scene->createLabels();
    scene->createButtons();

	scene->updateButtonState();

	 MenuItemImage *btn_fight = MenuItemImage::create("buttons/endbutton_notpushed.png", "buttons/endbutton_pushed.png", CC_CALLBACK_0(ShopScene::fight, scene));
	 btn_fight->setPosition(1400, 200);
	 scene->_menu->addChild(btn_fight);

    return scene;
}

void ShopScene::fight()
{
	_shopWindow->removeAllChildren();
	BossLevel *bosslvl = BossLevel::createBossDifficulty(UserDefault::getInstance()->getIntegerForKey("boss_difficulty"));
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, bosslvl, Color3B::BLACK));
}

int ShopScene::priceForColumn(int column)
{
	switch (column) {
		case 1: return 10;
		case 2: return 20;
		case 3: return 40;
		default: return 0;
	}
}

void ShopScene::createLabels() {
	Label *l1 = Label::createWithTTF("10", "fonts/Marker Felt.ttf", 60);
	Label *l2 = Label::createWithTTF("20", "fonts/Marker Felt.ttf", 60);
	Label *l3 = Label::createWithTTF("40", "fonts/Marker Felt.ttf", 60);
	l1->setPosition(180+BUTTON_SIZE+BUTTON_PADDING, 1005);
	l2->setPosition(l1->getPositionX()+BUTTON_SIZE+BUTTON_PADDING,l1->getPositionY());
	l3->setPosition(l2->getPositionX()+BUTTON_SIZE+BUTTON_PADDING,l2->getPositionY());

	Label *lshield = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 90);
	lshield->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
	lshield->setPosition(1120,1050); // 30px padding
	lshield->setColor(Color3B(42,42,165));
	lshield->setName("txt_shield_count");

	Label *lcredits = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 90);
	lcredits->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	lcredits->setPosition(1770,1050); // 30px padding
	lcredits->setColor(Color3B(165,42,42));
	lcredits->setName("txt_credits");

	this->addChild(l1);
	this->addChild(l2);
	this->addChild(l3);
	this->addChild(lshield);
	this->addChild(lcredits);
}

void ShopScene::createButtons()
{
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
			mii->setPosition(165 + column*(BUTTON_SIZE + BUTTON_PADDING), 165 + row*(BUTTON_SIZE + BUTTON_PADDING));
			mii->setTag((row<<2) + column + TAG_OFFSET);
			_menu->addChild(mii, 2);
		}
	}
}

void ShopScene::upgrade(Ref* p)
{
	int tag = ((MenuItemImage*)p)->getTag()-TAG_OFFSET;
	int row = tag>>2;
	int column = tag&3;

	perkUp(row, column);
	GameStateManager::addCoins(-priceForColumn(column));
	updateButtonState();
}

void ShopScene::showJosieInShopWindow(int row, int column){
	_shopWindow->removeAllChildren();
	_shopWindow->cleanup();
	BossPlayer* josie_model = BossPlayer::createWithLevel(nullptr);
	josie_model->setPosition(Vec2((1920/4) *3,1080/2 + 10));
	josie_model->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_shopWindow->addChild(josie_model);
}



//
// UserDefaults Manager
//
int ShopScene::perkGetValue(int row, int column) {
	UserDefault *ud = UserDefault::getInstance();
	switch (row) {
	case 3: return ud->getIntegerForKey("josie_perk_damage");
	case 2: return ud->getIntegerForKey("josie_perk_shoot");
	case 1: return ud->getIntegerForKey("josie_perk_frequency");
	default:
		switch (column) {
		case 3: return (int)ud->getBoolForKey("josie_perk_extralife");
		case 2: return ud->getIntegerForKey("josie_perk_shied");
		default: return ud->getIntegerForKey("josie_perk_playerspeed");
		}
	}
}

void ShopScene::perkSetValue(int row, int column, int newValue) {
	UserDefault *ud = UserDefault::getInstance();
	switch (row) {
	case 3: ud->setIntegerForKey("josie_perk_damage", newValue); break;
	case 2: ud->setIntegerForKey("josie_perk_shoot", newValue); break;
	case 1: ud->setIntegerForKey("josie_perk_frequency", newValue); break;
	default:
		switch (column) {
		case 3: ud->setBoolForKey("josie_perk_extralife", (bool)newValue); break;
		case 2: ud->setIntegerForKey("josie_perk_shied", newValue); break;
		default: ud->setIntegerForKey("josie_perk_playerspeed", newValue); break;
		}
	}
	ud->flush();
}

void ShopScene::perkUp(int row, int column) {
	AudioUnit::playShopPurchaseSound();
	perkSetValue(row, column, 1+ perkGetValue(row, column));
	showJosieInShopWindow(row, column);
}


//
// Button appearance
//
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

void ShopScene::updateButtonState()
{
	UserDefault *ud = UserDefault::getInstance();
	int credits = ud->getIntegerForKey("josie_credits");
	int shields = ud->getIntegerForKey("josie_perk_shied");
	std::ostringstream str_cr, str_sh;
	str_cr << credits;
	str_sh << shields;
	((Label*)this->getChildByName("txt_credits"))->setString(str_cr.str());
	((Label*)this->getChildByName("txt_shield_count"))->setString(str_sh.str());

	_equippedLayer->removeAllChildren();

	for (int i = 0; i<16; i++)
	{
		MenuItemImage *im = (MenuItemImage*)_menu->getChildByTag(i + TAG_OFFSET);
		int row = i>>2;
		int column = i&3;
		int perkVal = perkGetValue(row, column);

		if (i == 3) // extra life
		{
			if (perkVal) perkEquip(im);
			else if (credits >= priceForColumn(column)) perkEnable(im);
			else perkDisable(im);
			continue;
		}
		else if (i == 2) // shield
		{
			if (credits >= priceForColumn(column)) perkEnable(im);
			else perkDisable(im);
			continue;
		}

		if (perkVal > column) {
			perkEquip(im);
		} else if (perkVal == column && credits >= priceForColumn(column)) {
			perkEnable(im);
		} else {
			perkDisable(im);
		}
	}
}
