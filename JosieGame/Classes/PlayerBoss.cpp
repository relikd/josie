#include "PlayerBoss.h"

using namespace cocos2d;

PlayerBoss::PlayerBoss() {}
PlayerBoss::~PlayerBoss() {}

PlayerBoss* PlayerBoss::initWithLevel(Level* level)
{
	PlayerBoss *bp = new PlayerBoss();
	bp->initWithFile("josie/josie_transformed_static.png");
	bp->autorelease();
	return bp;
}
