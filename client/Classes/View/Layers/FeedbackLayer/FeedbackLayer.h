#ifndef __FeedbackLayer_H__
#define __FeedbackLayer_H__

#include "FeedbackQuestionLayer.h"
#include "../../ScrollLayer.h"
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class FeedbackLayer: public cocos2d::CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void backTouched(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC (FeedbackLayer);

private:
	void onTransmissionCompleted(CCHttpClient *sender, CCHttpResponse *response);
	ScrollLayer* _scrollLayer;
	cocos2d::CCLabelTTF* _loadingInfo;
	void sendFeedbackToServer(cocos2d::CCObject* pSender);
	std::vector<CCNode*> createQuestionObjects(float layerWidth);

	std::vector<FeedbackQuestionLayer*> _questions;
};

#endif // __StatsLayer_H__
