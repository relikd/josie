#ifndef DEBUG_COLLISION
#define DEBUG_COLLISION 0
#endif

#if !defined(Bogen)
#define Bogen(grad) ({ (grad)/180.0*M_PI; })
#endif


#include "CollisionLayer.h"


CollisionLayer::CollisionLayer() {
	spriteImage = nullptr;
	collisionType = CollisionLayerTypeGeneric;
	_target = nullptr;
}
CollisionLayer::~CollisionLayer() {}

CollisionLayer* CollisionLayer::createWithSize(float width, float height)
{
	CollisionLayer *cl = new CollisionLayer();
	cl->initCollisionSize(width, height);
	cl->autorelease();

	return cl;
}

CollisionLayer* CollisionLayer::createWithNode(Node* node) {
	CollisionLayer *other = CollisionLayer::createWithSize(node->getBoundingBox().size.width, node->getBoundingBox().size.height);
	other->setAnchorPoint(node->getAnchorPoint());
	other->setScale(node->getScale());
	other->setRotation(node->getRotation());
	other->setPosition(node->getPosition());
	return other;
}

bool CollisionLayer::initCollisionSize(float width, float height)
{
	if (initWithColor(Color4B(255,0,0,(DEBUG_COLLISION?255:0)), width, height)) {
		this->ignoreAnchorPointForPosition(false);
		return true;
	}
	return false;
}

// Custom Sprite Load

CollisionLayer* CollisionLayer::createCoinSprite()
{
	CollisionLayer *coin = CollisionLayer::createWithSize(50, 120);
	if (coin) {
		Vector<SpriteFrame*> frames;
		SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();

		char file[9] = { 0 };

		for (int i = 0; i < 6; i++) {
			sprintf(file, "coin%04d", i);
			SpriteFrame *frame = frameCache->getSpriteFrameByName(file);
			frames.pushBack(frame);
		}

		Animation *animation = Animation::createWithSpriteFrames(frames, 0.1);

		coin->insertSprite(Sprite::create());
		coin->spriteImage->setPosition(25,60);
		coin->setScale(0.4);
		coin->spriteImage->runAction(RepeatForever::create(Animate::create(animation)));
		coin->collisionType = CollisionLayerTypeCoin;
	}
	return coin;
}


//
// Collision Listener
//

void CollisionLayer::setCollisionListener(CollisionLayer* listener) {
	_target = listener;
	this->scheduleUpdate();
}

void CollisionLayer::removeCollisionListener() {
	_target = nullptr;
	this->unscheduleUpdate();
}

void CollisionLayer::update(float dt) {
	this->getCollision(_target);
}

//
// Insert Image Sprites
//

void CollisionLayer::insertImageName(const std::string& filename, Vec2 offset, Vec2 anchor)
{
	insertSprite(Sprite::create(filename), offset, anchor);
}

void CollisionLayer::insertImageFrameName(const std::string& name, Vec2 offset, Vec2 anchor)
{
	insertSprite(Sprite::createWithSpriteFrameName(name), offset, anchor);
}

void CollisionLayer::insertSprite(Sprite* sprite, Vec2 offset, Vec2 anchor)
{
	this->removeAllChildren();
	spriteImage = sprite;
	spriteImage->setAnchorPoint(anchor);
	spriteImage->setPosition(offset.x, offset.y);
	this->addChild(spriteImage);
}

bool CollisionLayer::getCollision(CollisionLayer* other) {
	if (other) {
		computeAxes();
		other->computeAxes();
		if (overlaps1Way(other) && other->overlaps1Way(this)) {
			other->hitByCollision(this); // tell the other part it is hit
			this->hitByCollision(other);
			return true;
		}
	}
	return false;
}

void CollisionLayer::hitByCollision(CollisionLayer* other)
{
	// override method
}

//
// PRIVATE collision detection with rotation and scale
//

bool CollisionLayer::overlaps1Way(CollisionLayer* other)
{
	for (int a = 0; a < 2; ++a) {

		double t = other->corner[0].dot(axis[a]);

		// Find the extent of box 2 on axis a
		double tMin = t;
		double tMax = t;

		for (int c = 1; c < 4; ++c) {
			t = other->corner[c].dot(axis[a]);

			if (t < tMin) {
				tMin = t;
			} else if (t > tMax) {
				tMax = t;
			}
		}

		// We have to subtract off the origin

		// See if [tMin, tMax] intersects [0, 1]
		if ((tMin > 1 + origin[a]) || (tMax < origin[a])) {
			// There was no intersection along this dimension;
			// the boxes cannot possibly overlap.
			return false;
		}
	}

	// There was no dimension along which there is no intersection.
	// Therefore the boxes overlap.
	return true;
}

/** Updates the axes after the corners move.  Assumes the
 corners actually form a rectangle. */
void CollisionLayer::computeAxes()
{
	Vec2 center = Vec2(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY());
	Size size_a = this->getContentSize() * this->getScale();
	float angle = Bogen(-this->getRotation());

	Vec2 X(cos(angle), sin(angle));
	Vec2 Y(-sin(angle), cos(angle));

	X *= size_a.width / 2;
	Y *= size_a.height / 2;

	corner[0] = center - X - Y;
	corner[1] = center + X - Y;
	corner[2] = center + X + Y;
	corner[3] = center - X + Y;

	axis[0] = corner[1] - corner[0];
	axis[1] = corner[3] - corner[0];

	// Make the length of each axis 1/edge length so we know any
	// dot product must be less than 1 to fall within the edge.

	for (int a = 0; a < 2; ++a) {
		float squaredLength = axis[a].x * axis[a].x + axis[a].y * axis[a].y;
		if (squaredLength != 0) {
			axis[a].x /= squaredLength;
			axis[a].y /= squaredLength;
		}
		origin[a] = corner[0].dot(axis[a]);
	}
}
