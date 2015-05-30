#ifndef SHOPSCENE_H_
#define SHOPSCENE_H_

#include "cocos2d.h"

class ShopScene : public cocos2d::Layer {
public:
	ShopScene();
	virtual ~ShopScene();
	virtual bool init();

	static cocos2d::Scene* createScene();
	CREATE_FUNC(ShopScene);

	void upgrade(Ref* p);
};

#endif /* SHOPSCENE_H_ */
