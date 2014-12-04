#ifndef __ScrollLayer_H__
#define __ScrollLayer_H__

#include "ScrollLayer.h"

#include "cocos2d.h"
using namespace cocos2d;

class ScrollLayer: public cocos2d::CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init(const char* title, std::vector<CCNode*> nodes, float layerWidth, float padding);
	static ScrollLayer* create(const char* title, std::vector<CCNode*> nodes, float layerWidth, float padding);
private:
	cocos2d::CCPoint initialTouch;
	cocos2d::CCPoint initialPos;
	float initialTouchDistance;
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* event);
	virtual void registerWithTouchDispatcher();
	void setThisPosition(float y);
	float _layer_min_y;
	float _layer_max_y;

	void update(float dt);
	cocos2d::CCPoint _lastTouch;
	double _lastTouchTime;
	bool _manualScroll;
	float _autoScrollSpeed;
};

#endif // __AchievementAchievementsLayer_H__
