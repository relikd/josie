#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize(1920,1080,ResolutionPolicy::FIXED_WIDTH);
    director->setContentScaleFactor(1.0);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);


    UserDefault *ud = UserDefault::getInstance();
    ud->setIntegerForKey("josie_credits", 9001);
    ud->setIntegerForKey("josie_perk_damage", 1);
    ud->setIntegerForKey("josie_perk_shoot", 1);
    ud->setIntegerForKey("josie_perk_frequency", 1);
    ud->setIntegerForKey("josie_perk_playerspeed", 1);
    ud->setIntegerForKey("josie_perk_shied", 0);
    ud->setBoolForKey("josie_perk_extralife", false);
    // ud->flush(); // no flush to reset settings after restart

    // preload animation frames
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("josie/josiewalk.plist", "josie/josiewalk.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("josie/josiestartmoving.plist", "josie/josiestartmoving.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("josie/josiejump.plist", "josie/josiejump.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tilemaps/coin.plist", "tilemaps/coin.png");


    // create a scene. it's an autorelease object
    auto scene = MainMenu::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

}
