#ifndef STAGEHAZARD_H_
#define STAGEHAZARD_H_

#include "CollisionLayer.h"

class StageHazard: public CollisionLayer {
public:
	StageHazard();
	virtual ~StageHazard();
	static StageHazard* createAt(const std::string& filename, Vec2 position, CollisionLayer* target);
	void fallDown();
private:
	void update(float dt);

	Vec2 _initialPosition;
	CollisionLayer* _target;
};

#endif /* STAGEHAZARD_H_ */
