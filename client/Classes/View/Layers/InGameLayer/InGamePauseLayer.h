#ifndef __InGamePauseLayer_H__
#define __InGamePauseLayer_H__

#include "cocos2d.h"

class InGamePauseLayer: public cocos2d::CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	CREATE_FUNC (InGamePauseLayer);


private:
	virtual bool init();
	// a selector callback
	void resumeTouched(cocos2d::CCObject* pSender);
	void pauseGame();
};

#endif // __InGamePauseLayer_H__
