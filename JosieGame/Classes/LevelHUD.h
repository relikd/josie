#ifndef LEVELHUD_H_
#define LEVELHUD_H_

#include "cocos2d.h"

class Player;

class LevelHUD : public cocos2d::Layer
{
public:
	LevelHUD();
	virtual ~LevelHUD();
	static LevelHUD* initWithLevelName(const std::string& name, Player* player);

	void setCoins(int count, int max);
private:
	cocos2d::Label *txt_coins;
	cocos2d::Label *txt_time;
	float _timeRunning;
	unsigned int _previousSeconds;

	void addStatusBar(const std::string& title);
	void addPlayerControls();
	void addPauseButton();
	void pauseGame();

	void updateStatusBarTime();
	void update(float dt);


	// Player Control
	Player* _player;
	cocos2d::MenuItemImage *_stay;
	cocos2d::MenuItemImage *_slide;
	cocos2d::MenuItemImage *_jump;
};

#endif /* LEVELHUD_H_ */
