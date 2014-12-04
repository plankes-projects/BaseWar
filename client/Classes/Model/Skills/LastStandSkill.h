/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef LastStandSkill_H_
#define LastStandSkill_H_

#include "Skill.h"
class LastStandSkill: public Skill {
public:
	virtual ~LastStandSkill();
	LastStandSkill(float manaCost, float rechargeTime, float onPercentLife, float duration, float increaseLifeByPercent,
			float increaseArmourByPercent);
	Skill* clone();
	virtual std::string getSkillInfo();
protected:
	bool virtual perform(Unit* owner);
private:
	float _onPercentLife;
	float _duration;
	float _increaseLifeByPercent;
	float _increaseArmourByPercent;
	void upgradeSkill(){
		_increaseLifeByPercent *= _upgradeMultiplier;
		_increaseArmourByPercent *= _upgradeMultiplier;
		_onPercentLife *= _upgradeMultiplier;
	}
};

#endif
