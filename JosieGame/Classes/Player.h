#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
    public:
        Player();
        ~Player();
        static Player* create();

        void initOptions();
        void update(float dt);

        void addEvents();
        void touchEvent(cocos2d::Touch* touch, cocos2d::Vec2 _p);

    private:

};

#endif // _PLAYER_H_
