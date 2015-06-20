/*
 * OptionScreen.h
 *
 *  Created on: 16.06.2015
 *      Author: danielmugge
 */

#ifndef OPTIONSCREEN_H_
#define OPTIONSCREEN_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class OptionScreen :public cocos2d::Layer {
public:
	OptionScreen();
	virtual ~OptionScreen();
	static OptionScreen* createOptionButton(float x, float y);

private:
	cocos2d::Layer *_overlay;
	void createOptionOverlay();
	cocos2d::ui::Slider *_music_volume_slider;
	cocos2d::ui::Slider *_sfx_volume_slider;

	void resetGameState();
	void toggleVisibility();

};

#endif /* OPTIONSCREEN_H_ */
