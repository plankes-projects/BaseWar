#ifndef __INGAMETOPMENULAYER_H__
#define __INGAMETOPMENULAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "InGameGameLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class InGameTopMenuLayer: public cocos2d::CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void toggleGUITouched(CCObject* pSender);
	void zoomInTouched(CCObject* pSender);
	void zoomOutTouched(CCObject* pSender);
	void autoCamTouched(CCObject* pSender);
	void centerTouched(CCObject* pSender);
	void pauseTouched(CCObject* pSender);
	void setInGamegameLayer(InGameGameLayer* ingameGameLayer);
	void shutdownTouched(CCObject* pSender);
	void shutdownFunction();
	void shutdownAbortFunction();
	void flipThisPlayerForCamTouched(CCObject* pSender);
	void enableSoundTouched(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC (InGameTopMenuLayer);

private:
	InGameGameLayer* ingameGameLayer;
	cocos2d::CCLabelTTF* _autCamOnOffLabel;

	cocos2d::CCLabelTTF* _enableSoundLabel;
	std::string _soundOnString;
	std::string _soundOffString;
};

#endif // __InGameTopMenuLayer_SCENE_H__
