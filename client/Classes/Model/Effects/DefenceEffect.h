/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef DefenceEffect_H_
#define DefenceEffect_H_

#include "Effect.h"
class Unit;

class DefenceEffect: public Effect {
public:
	virtual ~DefenceEffect();
	DefenceEffect(float timeInMilliseconds, float increasedHPInPercent, float increasedDefenseInPercent);
protected:
	virtual void perform(Unit* owner);
	virtual void onApply(Unit* owner);
	virtual void onRemoving(Unit* owner);

private:
	float _increasedHPInPercent;
	float _increasedDefenseInPercent;
};

#endif /* ATTACK_H_ */
