/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef RageEffect_H_
#define RageEffect_H_

#include "Effect.h"
class Unit;

class RageEffect: public Effect {
public:
	virtual ~RageEffect();
	RageEffect(float timeInMilliseconds, float increasedDamageInPercent, float increasedAttackSpeedInPercent,
			float increasedRunSpeedInPercent);
protected:
	virtual void perform(Unit* owner);
	virtual void onApply(Unit* owner);
	virtual void onRemoving(Unit* owner);

private:
	float _increasedDamageInPercent;
	float _increasedAttackSpeedInPercent;
	float _increasedRunSpeedInPercent;
};

#endif /* ATTACK_H_ */
