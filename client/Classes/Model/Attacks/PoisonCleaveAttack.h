/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef PoisonCleaveAttack_H_
#define PoisonCleaveAttack_H_

#include "Attack.h"
class PoisonCleaveAttack: public Attack {
public:
	virtual ~PoisonCleaveAttack();
	PoisonCleaveAttack(float poisonTime, float splashRange, int numberOfHitTargets, float damagePerSecond,
			DamageType damageType, float attackSpeed, float attackRange);
	virtual std::string getAttackInfo();
	virtual Attack* clone();
private:
	void virtual BAM(Unit* attacker, Unit* target);
	float _splashRange;
	int _numberOfHitTargets;
	float _poisonTime;
};

#endif /* INSTANTSPLASHATTACK_H_ */
