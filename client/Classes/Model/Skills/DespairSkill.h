/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef DespairSkill_H_
#define DespairSkill_H_

#include "Skill.h"
class DespairSkill: public Skill {
public:
	virtual ~DespairSkill();
	DespairSkill(float manaCost, float rechargeTime, float effectTime, float onPercentLife, float increasedDamageInPercent,
			float increasedAttackSpeedInPercent);
	Skill* clone();
	virtual std::string getSkillInfo();
protected:
	bool virtual perform(Unit* owner);
private:
	float _increasedDamageInPercent;
	float _increasedAttackSpeedInPercent;
	float _effectTime;
	float _onPercentLife;
	void upgradeSkill(){
		_increasedDamageInPercent *= _upgradeMultiplier;
		_increasedAttackSpeedInPercent *= _upgradeMultiplier;
		_onPercentLife *= _upgradeMultiplier;
	}
};

#endif
