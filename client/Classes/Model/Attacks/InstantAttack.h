/*
 * RangeAttack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef INSTANTATTACK_H_
#define INSTANTATTACK_H_

#include "Attack.h"

class InstantAttack: public Attack {
public:
	virtual ~InstantAttack();
	InstantAttack(float damagePerSecond, DamageType damageType, float attackSpeed, float attackRange);
	virtual std::string getAttackInfo();
	virtual Attack* clone();
private:
	void virtual BAM(Unit* attacker, Unit* target);
};

#endif /* INSTANTATTACK_H_ */
