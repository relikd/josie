#include "ShopScene.h"

using namespace cocos2d;
// temporary
int credits = 60;
int shot=2;
int dmg=1;
int freq=4;
int speed=1;
bool heart = 0;
bool shield = 1;

ShopScene::ShopScene() {}

ShopScene::~ShopScene() {}

Scene* ShopScene::createScene()
{
    auto scene = Scene::create();
    ShopScene *layer = (ShopScene*)ShopScene::create();

    Sprite *bg = Sprite::create("backgrounds/bg_shop.png");
    bg->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    layer->addChild(bg,1);

    layer->createButtons();

    scene->addChild(layer);
    return scene;
}

void ShopScene::createButtons()
{
	float btn_size = 200;
	float btn_padding = 30;
	float x1 = 165;
	float x2 = x1 + 1*(btn_size + btn_padding);
	float x3 = x1 + 2*(btn_size + btn_padding);
	float x4 = x1 + 3*(btn_size + btn_padding);
	float y1 = 165;
	float y2 = y1 + 1*(btn_size + btn_padding);
	float y3 = y1 + 2*(btn_size + btn_padding);
	float y4 = y1 + 3*(btn_size + btn_padding);


	MenuItemImage *btn_damage_1 = MenuItemImage::create("buttons/shopbuttons/dmg.png","buttons/shopbuttons/dmg.png", CC_CALLBACK_1(ShopScene::upgradeDamage, this));
	MenuItemImage *btn_damage_2 = MenuItemImage::create("buttons/shopbuttons/dmgX2.png","buttons/shopbuttons/dmgX2.png", CC_CALLBACK_1(ShopScene::upgradeDamage, this));
	MenuItemImage *btn_damage_3 = MenuItemImage::create("buttons/shopbuttons/dmgX3.png","buttons/shopbuttons/dmgX3.png", CC_CALLBACK_1(ShopScene::upgradeDamage, this));
	MenuItemImage *btn_damage_4 = MenuItemImage::create("buttons/shopbuttons/dmgX4.png","buttons/shopbuttons/dmgX4.png", CC_CALLBACK_1(ShopScene::upgradeDamage, this));
	btn_damage_1->setPosition(Point(x1,y4));
	btn_damage_2->setPosition(Point(x2,y4));
	btn_damage_3->setPosition(Point(x3,y4));
	btn_damage_4->setPosition(Point(x4,y4));

	MenuItemImage *btn_shoot_1 = MenuItemImage::create("buttons/shopbuttons/shot1.png","buttons/shopbuttons/shot1.png", CC_CALLBACK_1(ShopScene::upgradeShoot, this));
	MenuItemImage *btn_shoot_2 = MenuItemImage::create("buttons/shopbuttons/shot2.png","buttons/shopbuttons/shot2.png", CC_CALLBACK_1(ShopScene::upgradeShoot, this));
	MenuItemImage *btn_shoot_3 = MenuItemImage::create("buttons/shopbuttons/shot3.png","buttons/shopbuttons/shot3.png", CC_CALLBACK_1(ShopScene::upgradeShoot, this));
	MenuItemImage *btn_shoot_4 = MenuItemImage::create("buttons/shopbuttons/shot4.png","buttons/shopbuttons/shot4.png", CC_CALLBACK_1(ShopScene::upgradeShoot, this));
	btn_shoot_1->setPosition(Point(x1,y3));
	btn_shoot_2->setPosition(Point(x2,y3));
	btn_shoot_3->setPosition(Point(x3,y3));
	btn_shoot_4->setPosition(Point(x4,y3));

	MenuItemImage *btn_freq_1 = MenuItemImage::create("buttons/shopbuttons/frequency.png","buttons/shopbuttons/frequency.png", CC_CALLBACK_1(ShopScene::upgradeFrequency, this));
	MenuItemImage *btn_freq_2 = MenuItemImage::create("buttons/shopbuttons/frequencyX2.png","buttons/shopbuttons/frequencyX2.png", CC_CALLBACK_1(ShopScene::upgradeFrequency, this));
	MenuItemImage *btn_freq_3 = MenuItemImage::create("buttons/shopbuttons/frequencyX3.png","buttons/shopbuttons/frequencyX3.png", CC_CALLBACK_1(ShopScene::upgradeFrequency, this));
	MenuItemImage *btn_freq_4 = MenuItemImage::create("buttons/shopbuttons/frequencyX4.png","buttons/shopbuttons/frequencyX4.png", CC_CALLBACK_1(ShopScene::upgradeFrequency, this));
	btn_freq_1->setPosition(Point(x1,y2));
	btn_freq_2->setPosition(Point(x2,y2));
	btn_freq_3->setPosition(Point(x3,y2));
	btn_freq_4->setPosition(Point(x4,y2));

	MenuItemImage *btn_speed_1 = MenuItemImage::create("buttons/shopbuttons/movespeed.png","buttons/shopbuttons/movespeed.png", CC_CALLBACK_1(ShopScene::upgradeSpeed, this));
	MenuItemImage *btn_speed_2 = MenuItemImage::create("buttons/shopbuttons/movespeedX2.png","buttons/shopbuttons/movespeedX2.png", CC_CALLBACK_1(ShopScene::upgradeSpeed, this));
	MenuItemImage *btn_shield = MenuItemImage::create("buttons/shopbuttons/shield.png","buttons/shopbuttons/shield.png", CC_CALLBACK_1(ShopScene::upgradeShield, this));
	MenuItemImage *btn_life = MenuItemImage::create("buttons/shopbuttons/extralife.png","buttons/shopbuttons/extralife.png", CC_CALLBACK_1(ShopScene::upgradeHeart, this));
	btn_speed_1->setPosition(Point(x1,y1));
	btn_speed_2->setPosition(Point(x2,y1));
	btn_shield->setPosition(Point(x3,y1));
	btn_life->setPosition(Point(x4,y1));


	// assign the next button for activation
	btn_damage_2->setUserObject(btn_damage_3);
	btn_damage_3->setUserObject(btn_damage_4);
	btn_shoot_2->setUserObject(btn_shoot_3);
	btn_shoot_3->setUserObject(btn_shoot_4);
	btn_freq_2->setUserObject(btn_freq_3);
	btn_freq_3->setUserObject(btn_freq_4);

	Menu *menu = Menu::create(btn_shoot_1, btn_shoot_2, btn_shoot_3, btn_shoot_4, btn_damage_1, btn_damage_2, btn_damage_3, btn_damage_4, btn_freq_1, btn_freq_2, btn_freq_3, btn_freq_4, btn_speed_1, btn_speed_2, btn_shield, btn_life, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	perkDisable(btn_damage_2);
	perkDisable(btn_damage_3);
	perkDisable(btn_damage_4);
	perkDisable(btn_shoot_2);
	perkDisable(btn_shoot_3);
	perkDisable(btn_shoot_4);
	perkDisable(btn_freq_2);
	perkDisable(btn_freq_3);
	perkDisable(btn_freq_4);
	perkDisable(btn_speed_2);
	perkDisable(btn_shield);
	perkDisable(btn_life);

	// bought equipment
	perkEquip(btn_damage_1);
	if (dmg>1) perkEquip(btn_damage_2);
	if (dmg>2) perkEquip(btn_damage_3);
	if (dmg>3) perkEquip(btn_damage_4);
	perkEquip(btn_shoot_1);
	if (shot>1) perkEquip(btn_shoot_2);
	if (shot>2) perkEquip(btn_shoot_3);
	if (shot>3) perkEquip(btn_shoot_4);
	perkEquip(btn_freq_1);
	if (freq>1) perkEquip(btn_freq_2);
	if (freq>2) perkEquip(btn_freq_3);
	if (freq>3) perkEquip(btn_freq_4);
	perkEquip(btn_speed_1);
	if (speed>1) perkEquip(btn_speed_2);
	if (shield) perkEquip(btn_shield);
	if (heart) perkEquip(btn_life);

	// deactivate those with not enough money
	if (credits > 40) {
		if (dmg==3) perkEnable(btn_damage_4);
		if (shot==3) perkEnable(btn_shoot_4);
		if (freq==3) perkEnable(btn_freq_4);
		perkEnable(btn_life);
	}
	if (credits > 20) {
		if (dmg==2) perkEnable(btn_damage_3);
		if (shot==2) perkEnable(btn_shoot_3);
		if (freq==2) perkEnable(btn_freq_3);
		perkEnable(btn_shield);
	}
	if (credits > 10) {
		if (dmg==1) perkEnable(btn_damage_2);
		if (shot==1) perkEnable(btn_shoot_2);
		if (freq==1) perkEnable(btn_freq_2);
		if (speed==1) perkEnable(btn_speed_2);
	}
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
	this->addChild(frame, 2);

	MenuItemImage *nextButton = (MenuItemImage*)btn->getUserObject();
	if (nextButton) {
		perkEnable(nextButton);
	}
}

void ShopScene::upgradeDamage(Ref* p)
{
	perkEquip((MenuItemImage*)p);
	int price = (1<<dmg) * 10;
	CCLOG("price: %d", price);
	credits -= price;
	CCLOG("CREDITS: %d", credits);
	dmg++;
}

void ShopScene::upgradeShoot(Ref* p)
{
	perkEquip((MenuItemImage*)p);
	CCLOG("upgrade shoot");
}

void ShopScene::upgradeFrequency(Ref* p)
{
	perkEquip((MenuItemImage*)p);
	CCLOG("upgrade frequency");
}

void ShopScene::upgradeSpeed(Ref* p)
{
	perkEquip((MenuItemImage*)p);
	CCLOG("upgrade speed");
}

void ShopScene::upgradeShield(Ref* p)
{
	perkEquip((MenuItemImage*)p);
	CCLOG("upgrade shield");
}

void ShopScene::upgradeHeart(Ref* p)
{
	perkEquip((MenuItemImage*)p);
	CCLOG("upgrade life");
}
