#ifndef STAGEHAZARD_H_
#define STAGEHAZARD_H_

#include "CollisionLayer.h"

class StageHazard: public CollisionLayer {
public:
	StageHazard();
	virtual ~StageHazard();
	static StageHazard* createAt(Vec2 position);
	void setTarget(CollisionLayer* target) { _target = target; };
private:
	void update(float dt);
	void fallDown();

	Vec2 _initialPosition;
	CollisionLayer* _target;
};

#endif /* STAGEHAZARD_H_ */
