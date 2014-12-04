/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "DefenceEffect.h"
#include "../Units/Unit.h"

DefenceEffect::~DefenceEffect() {

}

DefenceEffect::DefenceEffect(float timeInMilliseconds, float increasedHPInPercent, float increasedDefenseInPercent) :
		Effect(timeInMilliseconds, NOTICK) {
	_increasedHPInPercent = increasedHPInPercent;
	_increasedDefenseInPercent = increasedDefenseInPercent;
	_isHarmful = false;
	_GUID = 4;
	_isStackAble = true; //if false, only duration is updated
}

void DefenceEffect::onApply(Unit* owner) {
	float grow = _increasedHPInPercent / 2;
	owner->increaseSizeBy(grow > 1 ? 1 : grow);
	owner->increaseHitpointsBy(_increasedHPInPercent);
	owner->increaseArmourEffectBy(_increasedDefenseInPercent);
}

void DefenceEffect::perform(Unit* owner) {
	//nothing to do
}

void DefenceEffect::onRemoving(Unit* owner) {
	float grow = _increasedHPInPercent / 2;
	owner->decreaseSizeBy(grow > 1 ? 1 : grow);
	owner->decreaseHitpointsBy(_increasedHPInPercent);
	owner->decreaseArmourEffectBy(_increasedDefenseInPercent);
}
