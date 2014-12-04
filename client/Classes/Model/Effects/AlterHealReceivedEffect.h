/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef AlterHealReceivedEffect_H_
#define AlterHealReceivedEffect_H_

#include "Effect.h"
class Unit;

class AlterHealReceivedEffect: public Effect {
public:
	virtual ~AlterHealReceivedEffect();
	AlterHealReceivedEffect(float timeInMilliseconds, float percent, bool isHarmful);
protected:
	virtual void perform(Unit* owner);
	virtual void onApply(Unit* owner);
	virtual void onRemoving(Unit* owner);
private:
	float _percent;
};

#endif /* ATTACK_H_ */
