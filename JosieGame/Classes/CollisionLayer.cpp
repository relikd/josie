#ifndef DEBUG_COLLISION
#define DEBUG_COLLISION 0
#endif

#if !defined(Bogen)
#define Bogen(grad) ({ (grad)/180.0*M_PI; })
#endif


#include "CollisionLayer.h"


CollisionLayer::CollisionLayer() {}
CollisionLayer::~CollisionLayer() {
	CCLOG("~CollisionLayer");
}


CollisionLayer* CollisionLayer::createWithSize(float width, float height)
{
	CollisionLayer *cl = new CollisionLayer();
	cl->initWithSize(width, height);
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

bool CollisionLayer::initWithSize(float width, float height)
{
	if (initWithColor(Color4B(255,0,0,(DEBUG_COLLISION?255:0)), width, height)) {
		this->ignoreAnchorPointForPosition(false);
		return true;
	}
	return false;
}


void CollisionLayer::insertImage(const std::string& filename, Vec2 anchor, Vec2 offset)
{
	Sprite *s = Sprite::create(filename);
	s->setAnchorPoint(anchor);
	s->setPosition(offset.x, offset.y);
	this->addChild(s);
}

bool CollisionLayer::getCollision(CollisionLayer* other) {
	computeAxes();
	other->computeAxes();
	return overlaps1Way(other) && other->overlaps1Way(this);
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
