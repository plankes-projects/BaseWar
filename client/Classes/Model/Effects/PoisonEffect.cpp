/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "PoisonEffect.h"
#include "../Units/Unit.h"

PoisonEffect::~PoisonEffect() {

}

PoisonEffect::PoisonEffect(float timeInMilliseconds, float tickEveryMilliSeconds, float dmgPerTick) :
		Effect(timeInMilliseconds, tickEveryMilliSeconds) {
	_damagePerTick = dmgPerTick;
	_isHarmful = true;
	_GUID = 2;
	_isStackAble = true; //if false, only duration is updated
}

void PoisonEffect::onApply(Unit* owner) {
	owner->addGreenColor();
}

void PoisonEffect::perform(Unit* owner) {
	owner->Receivedamage(_damagePerTick, MAGIC);
}

void PoisonEffect::onRemoving(Unit* owner) {
	owner->removeGreenColor();
}
