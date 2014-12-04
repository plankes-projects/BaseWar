/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef RageSkill_H_
#define RageSkill_H_

#include "Skill.h"
class RageSkill: public Skill {
public:
	virtual ~RageSkill();
	RageSkill(float manaCost, float rechargeTime, float range, float effectTime, float increasedDamageInPercent,
			float increasedAttackSpeedInPercent, float increasedRunSpeedInPercent);
	Skill* clone();
	virtual std::string getSkillInfo();
protected:
	bool virtual perform(Unit* owner);
private:
	float _range;
	float _increasedDamageInPercent;
	float _increasedAttackSpeedInPercent;
	float _increasedRunSpeedInPercent;
	float _effectTime;
	void upgradeSkill(){
		_increasedDamageInPercent *= _upgradeMultiplier;
		_increasedAttackSpeedInPercent *= _upgradeMultiplier;
		_increasedRunSpeedInPercent *= _upgradeMultiplier;
		_range *= _upgradeMultiplier;
	}
};

#endif
