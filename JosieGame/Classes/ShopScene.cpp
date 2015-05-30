#include "ShopScene.h"

using namespace cocos2d;

ShopScene::ShopScene() {}

ShopScene::~ShopScene() {}

bool ShopScene::init()
{
    if ( !Layer::init() ) return false;


    MenuItemImage *startbutton = MenuItemImage::create("buttons/startbutton.png",
       		"buttons/startbutton_pushed.png",
       		CC_CALLBACK_1(ShopScene::upgrade, this));
    startbutton->setPosition(Vec2(200,200));
    Size s = startbutton->getContentSize();
    startbutton->setScale(200/s.width, 200/s.height);

    auto menu = Menu::create(startbutton, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

Scene* ShopScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ShopScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


void ShopScene::upgrade(Ref* p)
{

}
