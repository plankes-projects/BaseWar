/*
 * BW_Animation.h
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#ifndef Healthbar_H_
#define Healthbar_H_

#include "cocos2d.h"
#include "../Tools/BW_Point.h"
#include "../Model/ArmyType.h"

class Healthbar: public cocos2d::CCLayer {
public:
	virtual bool init();
	void addManaBar();

	void setArmyType(ArmyType armyType);

	void updateHealthIndicator(float percent);
	void updateManaIndicator(float percent);

	void updateLifeDurationIndicator(float percent);

	float getHeigth();
	void addChampionMark();
	void addMasterMark();
	CREATE_FUNC (Healthbar);
protected:
private:

	cocos2d::CCSprite*  _hb_foreground;
	//cocos2d::CCSprite* _hb_background;
	cocos2d::CCLayerColor* _hb_health;
	cocos2d::CCLayerColor* _mb_mana;

	cocos2d::CCLayerColor* _lifeduration;

	float _hb_width;
	float _hb_height;
	float _mb_width;
	float _mb_height;

	float _lifeDuration_width;
	float _lifeDuration_height;

	float _barPadding;

	float _orig_health_content_width;
	float _orig_mana_content_width;
	float _orig_lifeDuration_content_width;

	float _orig_health_content_height;
	float _orig_mana_content_height;
	float _orig_lifeDuration_content_height;

	float _hp_y_pos;
	float _mana_y_pos;

	int _hpColorMaxGreen;
	int _hpColorMaxRed;
	int _hpColorBlue;
	int _hpColorAlpha;

	float _foreGroundBoarderSpace;
};

#endif /* Healthbar_H_ */

