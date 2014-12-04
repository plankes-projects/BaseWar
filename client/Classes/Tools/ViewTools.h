/*
 * Tools.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef VIEWTOOLS_H_
#define VIEWTOOLS_H_

#include "cocos2d.h"
using namespace cocos2d;
#include "../Model/Units/UnitFactory.h"
#include "../AchievementSystem/Achievement.h"

class ViewTools {
public:
	//returns the height scale
	static float addToMenu(CCMenu* menu, CCNode* menuItem, float menuItemWidth, float menuItemHeigth, float posx, float posy, float padding, int prio = 1);
	static void resizeMenu(CCMenu* menu, float menuItemWidth, float menuItemHeigth, int anzx, int anzy, float padding, float boarderPadding);


	static void showUnitInfo(UnitFactory* unitFactory, CCNode* parentNode);

	static float addVersionAndCreditInfo(CCNode* parentNode, float scaleMult);
	static void setStandardBackgroundImage(CCNode* parentNode);
	static void addIdleUnitTo(CCNode* parentNode, float offSetDown);
	static CCNode* createNotUnlockedMessage(int neededAchievementPoints);

	static CCLayerColor* createLayerFromAchievement(Achievement* ach);

	/**
	 * only call this functions on sprites which are on the gamefield!
	 */
	static bool isGameFieldSpriteOnScreen(CCSprite* sprite);

private:
	ViewTools();
	virtual ~ViewTools();
};

#endif /* ViewTools_H_ */
