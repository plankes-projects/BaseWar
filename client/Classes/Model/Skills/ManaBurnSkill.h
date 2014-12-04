/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef ManaBurnSkill_H_
#define ManaBurnSkill_H_

#include "Skill.h"
#include "../../Constants.h"
class ManaBurnSkill: public Skill {
public:
	virtual ~ManaBurnSkill();
	ManaBurnSkill(float manaCost, float rechargeTime, float range, float manaBurnPerManaUsed);
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
	float _manaBurnPerManaUsed;
	void upgradeSkill() {
		_manaBurnPerManaUsed *= _upgradeMultiplier;
		_range *= _upgradeMultiplier;
	}
};

#endif
