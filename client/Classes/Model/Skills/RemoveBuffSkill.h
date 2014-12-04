/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef RemoveBuffSkill_H_
#define RemoveBuffSkill_H_

#include "Skill.h"
#include "../../Constants.h"
class RemoveBuffSkill: public Skill {
public:
	virtual ~RemoveBuffSkill();
	RemoveBuffSkill(float manaCost, float rechargeTime, float range);
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
	void upgradeSkill(){
		_range *= _upgradeMultiplier;
	}
};

#endif
