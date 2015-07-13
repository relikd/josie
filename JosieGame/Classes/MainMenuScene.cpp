#include "MainMenuScene.h"
#include "Cutscene.h"
#include "LevelSelectScene.h"
#include "OptionScreen.h"
#include "TutorialScene.h"
#include "AudioUnit.h"

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
    if ( !Layer::init() )
    {
        return false;
    }
    
    AudioUnit::startBackgroundMenu();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	LayerColor* blueScreen = LayerColor::create(Color4B(97,154,196,255));
	this->addChild(blueScreen);
	
    Sprite* background = Sprite::create("backgrounds/bg_0.1.png");
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background->setPosition(Vec2(0, 0));
    this->addChild(background, 0);
	
    Sprite* josielogo = Sprite::create("main_menu_logo.png");
    josielogo->setScale(0.4);
    josielogo->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + visibleSize.height - josielogo->getContentSize().height*1.6));
    josielogo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->addChild(josielogo,1);

    Label* journey = Label::createWithTTF("A Jelly's Journey","fonts/Marker Felt.ttf", 50);
    journey->setColor(Color3B::BLACK);
    journey->setScale(0.0);
    journey->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    journey->setPosition(650, 50);
    journey->setRotation(-30.0);
    josielogo->addChild(journey);


    //StartScreen Animations
    cocos2d::ScaleTo* logo_scale = ScaleTo::create(1.5,2.0);
    cocos2d::EaseElasticIn* scale_easing = cocos2d::EaseElasticIn::create(logo_scale);
    cocos2d::ScaleTo* text_scale = ScaleTo::create(1.5,1.5);
    cocos2d::EaseElasticOut* elastic_text = cocos2d::EaseElasticOut::create(text_scale);
    CallFuncN* explode = CallFuncN::create(CC_CALLBACK_0(MainMenu::explode,this));
    CallFuncN* breathe = CallFuncN::create(CC_CALLBACK_0(MainMenu::breathAnimation,this, josielogo));
    cocos2d::Sequence* start_animation = Sequence::create(explode,scale_easing,journey->runAction(elastic_text),breathe,nullptr);
    josielogo->runAction(start_animation);



    MenuItemImage* tutorialbutton = MenuItemImage::create(
    										"buttons/menubuttons/tutorial_button.png",
											"buttons/menubuttons/tutorial_button_pushed.png",
											CC_CALLBACK_1(MainMenu::startTutorial,this));
    tutorialbutton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    tutorialbutton->setPosition(0,0);

	MenuItemImage* startbutton = MenuItemImage::create(
											"buttons/startbutton.png",
											"buttons/startbutton_pushed.png",
											CC_CALLBACK_1(MainMenu::play,this));
	startbutton->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y + (visibleSize.height/4.5)));
	startbutton->setScale(1.2);


	Menu* menu = Menu::create(startbutton,tutorialbutton, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setTag(101);
	this->addChild(menu, 1);




    //add OptionsScreenFunction
    this->addChild(OptionScreen::createOptionButton(1920,0),10);

    return true;
}



void MainMenu::play(Ref* pSender)
{
	AudioUnit::stopBackground();
	LevelSelect *levelSelectScreen = LevelSelect::createSceneWithLevel(1);
	Director::getInstance()->pushScene(levelSelectScreen);
}

void MainMenu::startTutorial(Ref* pSender){
	TutorialScene* tutScene = TutorialScene::initTutorial();
	Director::getInstance()->pushScene(tutScene);
}

void MainMenu::explode(){
    cocos2d::ParticleExplosion* hello2 = ParticleExplosion::createWithTotalParticles(200);
    hello2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    hello2->setPosition(Vec2(1920/2,200));
    hello2->setScale(3.0);
    this->addChild(hello2);
}

void MainMenu::breathAnimation(Sprite* logo){
	cocos2d::ScaleTo* logo_small_scaleback = ScaleTo::create(1.5,1.4);
	cocos2d::ScaleTo* logo_small_scaleup = ScaleTo::create(1.5,1.5);
	cocos2d::Sequence* logo_up_down = Sequence::createWithTwoActions(logo_small_scaleback,logo_small_scaleup);
	RepeatForever* breath = RepeatForever::create(logo_up_down);


    logo->runAction(breath);
}

