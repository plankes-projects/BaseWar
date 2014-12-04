#ifndef __AchievementLayer_H__
#define __AchievementLayer_H__

#include "cocos2d.h"
#include "../../ScrollLayer.h"
#include "AchievementUpdateType.h"

class AchievementLayer: public cocos2d::CCLayerColor {
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void updateAchievement(AchievementUpdateType type);

	// implement the "static node()" method manually
	CREATE_FUNC (AchievementLayer);
private:
	// a selector callback
	void backTouched(cocos2d::CCObject* pSender);
	void toggleModeTouched(cocos2d::CCObject* pSender);
	ScrollLayer* _achievementAchievementsLayer;
	AchievementUpdateType _type;
};

#endif // __AchievementLayer_H__
