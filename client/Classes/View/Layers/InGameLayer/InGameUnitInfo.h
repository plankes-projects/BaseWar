#ifndef INGAMEUNITINFO_H_
#define INGAMEUNITINFO_H_

#include "cocos2d.h"
#include "../../../Model/Units/UnitFactory.h"
#include "../../../Model/ArmyType.h"

class InGameUnitInfo: public cocos2d::CCLayerColor {
public:
	virtual bool init();
	~InGameUnitInfo();
	CREATE_FUNC (InGameUnitInfo);
	void initButtons(UnitFactory* unitFactory, float button_length,
			float button_heigth, float padding, cocos2d::CCSize parentMenuLayerSize);

	void popupRemoveParticleNO(CCObject *pSender);

private:

	void removeOld(ArmyType type);
	static InGameUnitInfo* _instanceRemoverLEFT;
	static InGameUnitInfo* _instanceRemoverRIGHT;

	void unitInfoTouched(cocos2d::CCObject* pSender);
	void shutdownTouched(cocos2d::CCObject* pSender);

	void update(float dt);

	UnitFactory* _unitFactory;
	cocos2d::CCMenuItemSprite* _unit;
	cocos2d::CCLabelTTF* _unitLabel;
	cocos2d::CCMenuItemSprite* _add;
	cocos2d::CCLabelTTF* _addLabel;
	cocos2d::CCMenuItemSprite* _sell;
	cocos2d::CCLabelTTF* _sellLabel;
	cocos2d::CCMenuItemSprite* _attackup;
	cocos2d::CCLabelTTF* _attackupLabel;
	cocos2d::CCMenuItemSprite* _armourup;
	cocos2d::CCLabelTTF* _armourupLabel;
	//cocos2d::CCMenuItemSprite* _info;
	cocos2d::CCMenuItemSprite* _runSpeedUp;
	cocos2d::CCLabelTTF* _runSpeedUpLabel;
	cocos2d::CCMenuItemSprite* _hitPointsUp;
	cocos2d::CCLabelTTF* _hitPointsUpLabel;

	cocos2d::CCMenuItemSprite* _manaUp;
	cocos2d::CCLabelTTF* _manaUpLabel;
	cocos2d::CCMenuItemSprite* _manaRegUp;
	cocos2d::CCLabelTTF* _manaRegLabel;
	cocos2d::CCMenuItemSprite* _skillUp;
	cocos2d::CCLabelTTF* _skillLabel;

	cocos2d::CCMenuItemSprite* _rangeUp;
	cocos2d::CCLabelTTF* _rangeLabel;
};

#endif /* INGAMEUNITINFO_H_ */
