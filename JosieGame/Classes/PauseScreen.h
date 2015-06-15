#ifndef PAUSESCREEN_H_
#define PAUSESCREEN_H_

#include "cocos2d.h"

class PauseScreen : public cocos2d::Layer {
public:
	PauseScreen();
	virtual ~PauseScreen();
	static PauseScreen* create();

	void createUI();

	void continueGame();
	void backToMenu();
};

#endif /* PAUSESCREEN_H_ */
