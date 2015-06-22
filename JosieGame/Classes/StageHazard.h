#ifndef STAGEHAZARD_H_
#define STAGEHAZARD_H_

#include "CollisionLayer.h"

class StageHazard: public CollisionLayer {
public:
	StageHazard();
	virtual ~StageHazard();
	static StageHazard* createAt(Vec2 position);
private:
	Vec2 _initialPosition;
	
	void fallDown();
	void hitByCollision(CollisionLayer* other);
};

#endif /* STAGEHAZARD_H_ */
