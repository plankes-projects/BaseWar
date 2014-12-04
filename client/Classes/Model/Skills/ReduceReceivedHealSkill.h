/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef ReduceReceivedHealSkill_H_
#define ReduceReceivedHealSkill_H_

#include "Skill.h"
#include "../../Constants.h"
class ReduceReceivedHealSkill: public Skill {
public:
	virtual ~ReduceReceivedHealSkill();
	ReduceReceivedHealSkill(float manaCost, float rechargeTime, float range, float effectTime, float percent);
	Skill* clone();
	virtual std::string getSkillInfo();

	virtual void turnIntoMaster() {
		_manaCost /= UNITS_TO_MASTER_UNIT_MULT;
		_rechargeTime /= UNITS_TO_MASTER_UNIT_MULT;
	}

	virtual void turnIntoUltimate() {
		_manaCost /= UNITS_TO_ULTIMATE_UNIT_MULT;
		_rechargeTime /= UNITS_TO_ULTIMATE_UNIT_MULT;
	}

protected:
	bool virtual perform(Unit* owner);
private:
	float _percent;
	float _effectTime;
	float _range;
	void upgradeSkill(){
		_range *= _upgradeMultiplier;
		_effectTime *= _upgradeMultiplier;
		_percent *= _upgradeMultiplier;
	}
};

#endif
