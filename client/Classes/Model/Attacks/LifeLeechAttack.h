/*
 * LifeLeechAttack.h
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#ifndef LIFELEECHATTACK_H_
#define LIFELEECHATTACK_H_

#include "Attack.h"

class LifeLeechAttack: public Attack {
public:
	virtual ~LifeLeechAttack();
	LifeLeechAttack(float lifeLeech, float damagePerSecond, DamageType damageType, float attackSpeed, float attackRange);
	virtual std::string getAttackInfo();
	virtual Attack* clone();
private:
	void virtual BAM(Unit* attacker, Unit* target);
	float _lifeLeech;
};

#endif /* LIFELEECHATTACK_H_ */
