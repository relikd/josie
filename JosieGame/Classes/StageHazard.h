/*
 * StageHazard.h
 *
 *  Created on: 20.06.2015
 *      Author: Jonas
 */

#ifndef STAGEHAZARD_H_
#define STAGEHAZARD_H_

#include "CollisionLayer.h"
class LevelPlayer;

class StageHazard: public CollisionLayer {
public:
	StageHazard();
	virtual ~StageHazard();
	static StageHazard* createAt(const std::string& filename, Vec2 position,
			 LevelPlayer* target);
	void reset();
	void fallDown();
private:
	void update(float dt);

	Vec2 _Position;
	LevelPlayer* _target;
};

#endif /* STAGEHAZARD_H_ */
