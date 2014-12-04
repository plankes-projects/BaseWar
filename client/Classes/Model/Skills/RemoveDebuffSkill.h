/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef RemoveDebuffSkill_H_
#define RemoveDebuffSkill_H_

#include "Skill.h"
#include "../../Constants.h"
class RemoveDebuffSkill: public Skill {
public:
	virtual ~RemoveDebuffSkill();
	RemoveDebuffSkill(float manaCost, float rechargeTime, float range, float immuneDuration);
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
	float _range;
	float _immuneDuration;
	void upgradeSkill(){
		_immuneDuration *= _upgradeMultiplier;
		_range *= _upgradeMultiplier;
	}
};

#endif
