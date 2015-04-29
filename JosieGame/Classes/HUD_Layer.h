/*
 * HUD_Layer.h
 *
 *  Created on: 29.04.2015
 *      Author: Jonas
 */

#ifndef HUD_Layer_H_
#define HUD_Layer_H_

#include "cocos2d.h"
class Level;
class PlayerControl;

namespace cocos2d {

class HUD_Layer : public cocos2d::Layer {
public:
	HUD_Layer();
	virtual ~HUD_Layer();

	static HUD_Layer* createForLevel(Level* level);

	void addBackground(Level* level);
	void addPauseButton(Level* level);
	void addPlayerControl(Level* level);
};

} /* namespace cocos2d */

#endif /* HUD_Layer_H_ */
