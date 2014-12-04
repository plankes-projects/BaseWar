/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef IceEffect_H_
#define IceEffect_H_

#include "Effect.h"
class Unit;

class IceEffect: public Effect {
public:
	virtual ~IceEffect();
	IceEffect(float timeInMilliseconds, float decreasedAttackSpeedInPercent,
			float decreasedRunSpeedInPercent);
protected:
	virtual void perform(Unit* owner);
	virtual void onApply(Unit* owner);
	virtual void onRemoving(Unit* owner);

private:
	float _decreasedAttackSpeedInPercent;
	float _decreasedRunSpeedInPercent;
};

#endif /* ATTACK_H_ */
