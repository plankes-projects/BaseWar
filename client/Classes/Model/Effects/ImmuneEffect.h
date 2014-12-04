/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef ImmuneEffect_H_
#define ImmuneEffect_H_

#include "Effect.h"
class Unit;

class ImmuneEffect: public Effect {
public:
	virtual ~ImmuneEffect();
	ImmuneEffect(float timeInMilliseconds, int effectImmunityGUID);
protected:
	virtual void perform(Unit* owner);
	virtual void onApply(Unit* owner);
	virtual void onRemoving(Unit* owner);

private:
	int _effectImmunityGUID;
};

#endif /* ImmuneEffect_H_ */
