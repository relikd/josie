#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "AudioUnit.h"
#include "GameStateManager.h"

using namespace cocos2d;

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

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
        glview = GLViewImpl::create("Josie Game");
        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize(1920,1080,ResolutionPolicy::FIXED_WIDTH);
    director->setContentScaleFactor(1.0);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    if (UserDefault::getInstance()->getBoolForKey("josie_save_state", false)==false){
    GameStateManager::initSpriteCache();
    }

    director->runWithScene(MainMenu::createScene());

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    AudioUnit::pauseBackground();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    AudioUnit::resumeBackground();
}
