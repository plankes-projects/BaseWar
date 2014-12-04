/*
 * InstantHeal.h
 *
 *  Created on: 28.05.2013
 *      Author: Planke
 */

#ifndef INSTANTHEAL_H_
#define INSTANTHEAL_H_

#include "Attack.h"

class InstantHeal: public Attack {
public:
	InstantHeal(float healPerSecond, DamageType healType, float healSpeed, float healRange);
	virtual ~InstantHeal();
	virtual std::string getAttackInfo();
	virtual Attack* clone();
private:
	void virtual BAM(Unit* attacker, Unit* target);
};

#endif /* INSTANTHEAL_H_ */
