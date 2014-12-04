/*
* InGameBottomMenuLayer.h
*
*  Created on: May 20, 2013
*      Author: planke
*/

#ifndef INGAMEBOTTOMMENULAYER_H_
#define INGAMEBOTTOMMENULAYER_H_

#include "cocos2d.h"
#include "../../../Model/Units/UnitFactory.h"
#include "../../../Model/Units/Tier.h"
#include "../../../Model/Player.h"

class InGameBottomMenuLayer: public cocos2d::CCLayerColor {
public:
	virtual bool init();

	CREATE_FUNC (InGameBottomMenuLayer);

private:
	void update(float dt);
	std::list<cocos2d::CCMenuItemSprite*> _unitButtonList;
	void menuTouched(CCObject* pSender);
	void showUnitLayerTouched(CCObject* pSender);
	void updateToTier2(CCObject* pSender);
	void updateToTier3(CCObject* pSender);
	void updateGoldLabel(float dt);

	bool _menuIsHidden;
	cocos2d::CCPoint _hiddenLocation;
	cocos2d::CCPoint _shownLocation;
	cocos2d::CCMenuItemSprite* _incomeUpdateButton;
	cocos2d::CCLabelTTF* _incomeUpdateButtonLabel;
	cocos2d::CCMenuItemSprite* _tier2Button;
	cocos2d::CCLabelTTF* _tier2ButtonLabel;
	cocos2d::CCMenuItemSprite* _tier3Button;
	cocos2d::CCLabelTTF* _tier3ButtonLabel;
	Player* _owner;

	float _button_length;
	float _button_heigth;
	float _padding;
	float _speed;
	float _menuButtonWidth;
	cocos2d::CCLabelTTF* _gold_label;

	cocos2d::CCMenu* createTierMenu();


	bool _unitInfoCalled;
	bool _unitInfoMenuCalled;
};

#endif /* INGAMEBOTTOMMENULAYER_H_ */
