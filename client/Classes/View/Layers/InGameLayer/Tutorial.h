#ifndef __Tutorial_H__
#define __Tutorial_H__

#include "cocos2d.h"
using namespace cocos2d;

class Tutorial: public CCObject {
public:
	Tutorial(CCNode* layer);
	void gameFieldTut();
	void unitMenuInfo();
	void unitInfo();
	void afterUnitInfoWatched(cocos2d::CCObject* pSender);
	void afterUnitInfoWatched2(cocos2d::CCObject* pSender);

private:
	//gameFieldTut
	void TowerKillInfo(cocos2d::CCObject* pSender);
	void MiddleControlInfo(cocos2d::CCObject* pSender);
	void goldInfo(cocos2d::CCObject* pSender);

	//unitMenuInfo
	void unitMenuInfo2(cocos2d::CCObject* pSender);

	//unitInfo
	void unitInfo2(cocos2d::CCObject* pSender);

	void TopMenuInfo(cocos2d::CCObject* pSender);
	void SpawnInfo(cocos2d::CCObject* pSender);


	void tutorialEndMessage(cocos2d::CCObject* pSender);

	CCNode* _scene;
};

#endif
