/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef PoisonEffect_H_
#define PoisonEffect_H_

#include "Effect.h"
class Unit;

class PoisonEffect: public Effect {
public:
	virtual ~PoisonEffect();
	PoisonEffect(float timeInMilliseconds, float tickEveryMilliSeconds, float dmgPerTick);
protected:
	virtual void perform(Unit* owner);
	virtual void onApply(Unit* owner);
	virtual void onRemoving(Unit* owner);

private:
	float _damagePerTick;
};

#endif /* ATTACK_H_ */
