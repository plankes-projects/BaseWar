#ifndef __STARTMENULAYER_H__
#define __STARTMENULAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

class StartMenuLayer: public cocos2d::CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void oneVsOneTouched(cocos2d::CCObject* pSender);
	void networkGameTouched(cocos2d::CCObject* pSender);
	void shutdownTouched(cocos2d::CCObject* pSender);
	void oneVsCompTouched(cocos2d::CCObject* pSender);
	void statsTouched(cocos2d::CCObject* pSender);
	void feedbackTouched(cocos2d::CCObject* pSender);
	void tutorialTouched(cocos2d::CCObject* pSender);
	void voidTouched(cocos2d::CCObject* pSender);
	void achievementTouched(cocos2d::CCObject* pSender);
	void enableSoundTouched(cocos2d::CCObject* pSender);
#ifdef DEBUG
	void startBalanceGameTouched(cocos2d::CCObject* pSender);
	void startNextRoundIfNeeded(float dt);
#endif
	CCEditBox* _playerNameTextBox;

	// implement the "static node()" method manually
	CREATE_FUNC (StartMenuLayer);

private:
	cocos2d::CCLabelTTF* _enableSoundLabel;
	std::string _soundOnString;
	std::string _soundOffString;
};

#endif // __STARTMENULAYER_H__
