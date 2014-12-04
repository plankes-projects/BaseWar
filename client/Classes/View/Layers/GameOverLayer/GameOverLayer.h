#ifndef __GAMEOVERLAYER_H__
#define __GAMEOVERLAYER_H__

#include "cocos2d.h"

class GameOverLayer: public cocos2d::CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void leaveTouched(cocos2d::CCObject* pSender);
	virtual void ccTouchesBegan(cocos2d::CCSet * pTouches,
			cocos2d::CCEvent * pEvent);

	// implement the "static node()" method manually
	CREATE_FUNC (GameOverLayer);

	void setWon(bool won);

};

#endif // __GAMEOVERLAYER_H__
