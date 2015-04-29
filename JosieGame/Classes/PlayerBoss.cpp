#include "PlayerBoss.h"
#include "Projectile.h"
#include "Level.h"

using namespace cocos2d;


const float walkspeed = 11.0;

PlayerBoss::PlayerBoss() {
	_level = NULL;
}
PlayerBoss::~PlayerBoss() {}

PlayerBoss* PlayerBoss::initWithLevel(Level* level)
{
	PlayerBoss *bp = new PlayerBoss();
	bp->initWithFile("josie/josie_transformed_static.png");
	bp->autorelease();
	bp->setAnchorPoint(Vec2(0.5,0));
	bp->_level = level;
	return bp;
}

void PlayerBoss::moveLeft()
{
	this->setPositionX( this->getPositionX() - walkspeed );
}
void PlayerBoss::moveRight()
{
	this->setPositionX( this->getPositionX() + walkspeed );
}
void PlayerBoss::shoot()
{
	Projectile* projectile = Projectile::init(0,0);
	projectile->setPosition(this->getPositionX(), this->getPositionY() + this->getContentSize().height);
	_level->addChild(projectile,1);
	projectile->runAction(MoveTo::create(3.0f,Vec2(this->getPositionX(),900)));

	//projectile->killProjectile();


}
void PlayerBoss::jump()
{
	// TODO: jump
}
