#ifndef __LOADINGSPRITELAYER_H__
#define __LOADINGSPRITELAYER_H__

#include "cocos2d.h"
using namespace cocos2d;

class LoadingSpriteLayer: public cocos2d::CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void load(float dt);

	// implement the "static node()" method manually
	CREATE_FUNC (LoadingSpriteLayer);

private:
	std::string getRandomgameTip();
	bool _loadingCalled;
	std::list<std::string> _toLoad;
	int _count;
	int _counter;
	void setPercent(float percent);
	CCProgressTimer* _backGroundProgress;
};

#endif // __LOADINGSPRITELAYER_H__
