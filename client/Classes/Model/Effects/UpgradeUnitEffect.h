/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef UpgradeUnitEffect_H_
#define UpgradeUnitEffect_H_

#include "Effect.h"
class Unit;

class UpgradeUnitEffect: public Effect {
public:
	virtual ~UpgradeUnitEffect();
	UpgradeUnitEffect(float timeInMilliseconds, float tickEveryMilliSeconds);
protected:
	virtual void perform(Unit* owner);
	virtual void onApply(Unit* owner);
	virtual void onRemoving(Unit* owner);
};

#endif /* ATTACK_H_ */
