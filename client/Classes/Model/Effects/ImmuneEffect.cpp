/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "ImmuneEffect.h"
#include "../Units/Unit.h"

ImmuneEffect::~ImmuneEffect() {

}

ImmuneEffect::ImmuneEffect(float timeInMilliseconds, int effectImmunityGUID) :
		Effect(timeInMilliseconds, NOTICK) {
	_isHarmful = false;
	_GUID = 5;
	_isStackAble = true; //if false, only duration is updated
	_effectImmunityGUID = effectImmunityGUID;
}

void ImmuneEffect::onApply(Unit* owner) {
	owner->addEffectImmunity(_effectImmunityGUID);
}

void ImmuneEffect::perform(Unit* owner) {
	//nothing to do
}

void ImmuneEffect::onRemoving(Unit* owner) {
	owner->removeEffectImmunity(_effectImmunityGUID);
}
