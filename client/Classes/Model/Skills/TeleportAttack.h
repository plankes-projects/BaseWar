/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef TeleportAttack_H_
#define TeleportAttack_H_

#include "Skill.h"
class TeleportAttack: public Skill {
public:
	virtual ~TeleportAttack();
	TeleportAttack(float manaCost, float rechargeTime, float range, float damageMultiplier = 2);
	Skill* clone();
	virtual std::string getSkillInfo();
protected:
	bool virtual perform(Unit* owner);
private:
	float _range;
	float _damageMultiplier;
	void upgradeSkill(){
		_damageMultiplier *= _upgradeMultiplier;
		_range *= _upgradeMultiplier;
	}
};

#endif
