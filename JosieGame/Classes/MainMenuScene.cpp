#include "MainMenuScene.h"
#include "Cutscene.h"
#include "LevelSelectScene.h"
#include "OptionScreen.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* background = Sprite::create("backgrounds/bg_mountain72dpi.png");
    background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(background, 0);

    Sprite* josielogo = Sprite::create("main_menu_logo.png");
    josielogo->setScale(0.4);
    josielogo->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - josielogo->getContentSize().height*1.5));
    josielogo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->addChild(josielogo,1);

    Label* journey = Label::createWithTTF("A Jelly's Journey","fonts/Marker Felt.ttf", 50);
    journey->setColor(Color3B::BLACK);
    journey->setScale(0.0);
    journey->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    journey->setPosition(650, 50);
    journey->setRotation(-30.0);
    josielogo->addChild(journey);

    cocos2d::ScaleTo* logo_scale = ScaleTo::create(1.5,2.0);
    cocos2d::EaseElasticIn* scale_easing = cocos2d::EaseElasticIn::create(logo_scale);
    CallFuncN* explode = CallFuncN::create(CC_CALLBACK_0(MainMenu::explode,this));
    cocos2d::ScaleTo* text_scale = ScaleTo::create(1.5,1.5);
    cocos2d::EaseElasticOut* elastic_text = cocos2d::EaseElasticOut::create(text_scale);


    cocos2d::Sequence* start_animation = Sequence::create(scale_easing,explode,journey->runAction(elastic_text),nullptr);
    josielogo->runAction(start_animation);







	MenuItemImage* startbutton = MenuItemImage::create(
											"buttons/startbutton.png",
											"buttons/startbutton_pushed.png",
											CC_CALLBACK_1(MainMenu::play,this));
	startbutton->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + (visibleSize.height/4)));


	Menu* menu = Menu::create(startbutton, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setTag(101);
	this->addChild(menu, 1);




    //add OptionsScreenFunction
    this->addChild(OptionScreen::createOptionButton(1900,20),10);

    return true;
}



void MainMenu::play(Ref* pSender)
{
	LevelSelect *levelSelectScreen = LevelSelect::createSceneWithLevel(1);
	Director::getInstance()->pushScene(levelSelectScreen);
}

void MainMenu::explode(){
    cocos2d::ParticleExplosion* hello2 = ParticleExplosion::createWithTotalParticles(200);
    hello2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    hello2->setPosition(Vec2(1920/2,200));
    hello2->setScale(3.0);
    this->addChild(hello2);
}

