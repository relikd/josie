#ifndef LEVELSELECTSCENE_H_
#define LEVELSELECTSCENE_H_

#include "cocos2d.h"

class LevelSelect : public cocos2d::Scene {
public:
	LevelSelect();
	virtual ~LevelSelect();

	static LevelSelect* createSceneWithLevel(int level);
	void buildUI();
	cocos2d::MenuItemImage *buttonWithSublevel(int sublevel);

	void startLevel(int sublevel);
	void startRandomLevel();
private:
	int _level;
};


#endif /* LEVELSELECTSCENE_H_ */
