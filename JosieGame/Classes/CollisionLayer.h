#ifndef COLLISIONLAYER_H_
#define COLLISIONLAYER_H_

#include "cocos2d.h"

using namespace cocos2d;

class CollisionLayer : public cocos2d::LayerColor {
public:
	CollisionLayer();
	virtual ~CollisionLayer();

	static CollisionLayer* createWithSize(float width, float height);
	static CollisionLayer* createWithNode(Node* node);
	bool initCollisionSize(float width, float height);

	// Custom Sprite Load
	static CollisionLayer* createCoinSprite();

	Sprite *spriteImage;
	void insertImageName(const std::string& filename, Vec2 offset = Vec2::ZERO, Vec2 anchor = Vec2::ANCHOR_MIDDLE);
	void insertImageFrameName(const std::string& name, Vec2 offset = Vec2::ZERO, Vec2 anchor = Vec2::ANCHOR_MIDDLE);
	void insertSprite(Sprite* sprite, Vec2 offset = Vec2::ZERO, Vec2 anchor = Vec2::ANCHOR_MIDDLE);

	bool getCollision(CollisionLayer* other);
private:
	cocos2d::Vec2 corner[4];
	cocos2d::Vec2 axis[2];
	double origin[2];
	bool overlaps1Way(CollisionLayer* other);
	void computeAxes();
};

#endif /* COLLISIONLAYER_H_ */
