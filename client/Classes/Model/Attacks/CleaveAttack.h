/*
 * InstantSplashAttack.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef CLEAVEATTACK_H_
#define CLEAVEATTACK_H_

#include "Attack.h"
class CleaveAttack: public Attack {
public:
	virtual ~CleaveAttack();
	CleaveAttack(float splashRange, int numberOfAdditionalHitTargets, float splashDamageInPercent, float damagePerSecond,
			DamageType damageType, float attackSpeed, float attackRange);
	virtual std::string getAttackInfo();
	virtual Attack* clone();
private:
	void virtual BAM(Unit* attacker, Unit* target);
	float _splashRange;
	int _numberOfAdditionalHitTargets;
	float _splashDamageInPercent;
};

#endif /* INSTANTSPLASHATTACK_H_ */
