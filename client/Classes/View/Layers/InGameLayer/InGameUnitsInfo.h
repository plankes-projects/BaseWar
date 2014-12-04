#ifndef INGAMEUNITSINFO_H_
#define INGAMEUNITSINFO_H_

#include "cocos2d.h"
#include "../../../Model/Player.h"
#include "../../CCConfirmView.h"

class InGameUnitsInfo: public cocos2d::CCLayerColor {
public:
	virtual bool init();
	CREATE_FUNC (InGameUnitsInfo);

private:
	void addUnitInfosForPlayer(CCMenu* menu, Player* player, ArmyType side);
	void addButtonInfo(CCMenu* menu);
	void unitInfoTouched(cocos2d::CCObject* pSender);
	void hideAnimation();
	void showAnimation();
	float _buttonYPosition;
	float _buttonYPositionCorrectionSpaceForPlayerName;
	float _menuItemWidth;
	float _menuItemHeigth;
	float _padding;
	std::vector<UnitFactory*> allFactories; //we dont need to delete this
	int _unitFactoryPos;

	cocos2d::CCPoint _hiddenLocation;
	cocos2d::CCPoint _shownLocation;
	bool _menuIsHidden;
	float _speed;

	cocos2d::CCPoint initialTouch;
	cocos2d::CCPoint initialPos;
	float initialTouchDistance;
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* event);
	virtual void registerWithTouchDispatcher();

	float _infoRowOffsetX;
	float _infoRowOffsetY;

	void attackInfoTouched(cocos2d::CCObject* pSender){
		CCConfirmView *alert = CCConfirmView::create("Attack upgrade", "Your unit will hit harder.", "Ok");
		getParent()->addChild(alert, 100);
	}

	void armorInfoTouched(cocos2d::CCObject* pSender){
		CCConfirmView *alert = CCConfirmView::create("Armor upgrade", "Your unit will get less damage.", "Ok");
		getParent()->addChild(alert, 100);
	}

	void healthInfoTouched(cocos2d::CCObject* pSender){
		CCConfirmView *alert = CCConfirmView::create("Hitpoints upgrade", "Your unit will live longer.", "Ok");
		getParent()->addChild(alert, 100);
	}

	void runInfoTouched(cocos2d::CCObject* pSender){
		CCConfirmView *alert = CCConfirmView::create("Run speed upgrade", "Your unit will run faster.", "Ok");
		getParent()->addChild(alert, 100);
	}

	void skillInfoTouched(cocos2d::CCObject* pSender){
		CCConfirmView *alert = CCConfirmView::create("Skills upgrade", "Skills of your unit will be more effective.", "Ok");
		getParent()->addChild(alert, 100);
	}

	void manaInfoTouched(cocos2d::CCObject* pSender){
		CCConfirmView *alert = CCConfirmView::create("Mana upgrade", "Skills of your unit will be used more often.", "Ok");
		getParent()->addChild(alert, 100);
	}

	void manaRegInfoTouched(cocos2d::CCObject* pSender){
		CCConfirmView *alert = CCConfirmView::create("Mana regeneration upgrade", "Skills of your unit will be used more often.", "Ok");
		getParent()->addChild(alert, 100);
	}

	void rangeInfoTouched(cocos2d::CCObject* pSender){
		CCConfirmView *alert = CCConfirmView::create("Range upgrade", "Your unit will have a greater attack range.", "Ok");
		getParent()->addChild(alert, 100);
	}
};

#endif
