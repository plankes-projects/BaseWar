/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "RageEffect.h"
#include "../Units/Unit.h"

RageEffect::~RageEffect() {

}

RageEffect::RageEffect(float timeInMilliseconds, float increasedDamageInPercent, float increasedAttackSpeedInPercent,
		float increasedRunSpeedInPercent) :
		Effect(timeInMilliseconds, NOTICK) {
	_increasedDamageInPercent = increasedDamageInPercent;
	_increasedAttackSpeedInPercent = increasedAttackSpeedInPercent;
	_increasedRunSpeedInPercent = increasedRunSpeedInPercent;
	_isHarmful = false;
	_GUID = 1;
	_isStackAble = true; //if false, only duration is updated
}

void RageEffect::onApply(Unit* owner) {
	owner->addRedColor();
	owner->increaseAttackDamageBy(_increasedDamageInPercent);
	owner->increaseAttackSpeedBy(_increasedAttackSpeedInPercent);
	owner->increaseRunSpeedBy(_increasedRunSpeedInPercent);
}

void RageEffect::perform(Unit* owner) {
	//nothing to do
}

void RageEffect::onRemoving(Unit* owner) {
	owner->removeRedColor();
	owner->decreaseAttackDamageBy(_increasedDamageInPercent);
	owner->decreaseAttackSpeedBy(_increasedAttackSpeedInPercent);
	owner->decreaseRunSpeedBy(_increasedRunSpeedInPercent);
}
