#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class MainMenu : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene();
    virtual bool init();
    
	void play(cocos2d::Ref* pSender);
	void explode();
    CREATE_FUNC(MainMenu);


};

#endif // __MAINMENU_SCENE_H__
