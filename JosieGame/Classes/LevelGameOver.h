#ifndef LEVELGAMEOVER_H_
#define LEVELGAMEOVER_H_

#include "cocos2d.h"

class LevelGameOver : public cocos2d::Scene
{
public:
	LevelGameOver();
	virtual ~LevelGameOver();

	static LevelGameOver* createWin(int coins, int max_coins, float time);
	static LevelGameOver* createFail();
private:
	void addSuccess(int coins, int max_coins, float time);
	void addFail();
	void backToMenu();
};

#endif /* LEVELGAMEOVER_H_ */
