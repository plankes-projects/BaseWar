#ifndef __StatsLayer_H__
#define __StatsLayer_H__

#include "cocos2d.h"
using namespace cocos2d;

class StatsLayer: public cocos2d::CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static node()" method manually
	CREATE_FUNC (StatsLayer);

private:
	CCLayerColor* createSectionLayer(std::vector<std::string> section, float width);

	// a selector callback
	void backTouched(cocos2d::CCObject* pSender);
};

#endif // __StatsLayer_H__
