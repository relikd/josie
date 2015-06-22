#ifndef LEVELHUD_H_
#define LEVELHUD_H_

#include "cocos2d.h"

class LevelHUD : public cocos2d::Layer
{
public:
	LevelHUD();
	virtual ~LevelHUD();
	static LevelHUD* initWithLevelName(const std::string& name);

	void setCoins(int count, int max);
	void setTime(float time) { _timeRunning = time; updateStatusBarTime(true); };
	float getTime() { return _timeRunning; };
private:
	cocos2d::Label *txt_coins;
	cocos2d::Label *txt_time;
	float _timeRunning;
	unsigned int _previousSeconds;

	void addStatusBar(const std::string& title);
	void addPlayerControls();

	void updateStatusBarTime(bool force=false);
	void update(float dt);


	// Player Control
	cocos2d::MenuItemImage *_stay;
	cocos2d::MenuItemImage *_slide;
	cocos2d::MenuItemImage *_jump;
	
	void addKeyboardListener();
	bool _key_stay, _key_slide, _key_jump;
};

#endif /* LEVELHUD_H_ */
