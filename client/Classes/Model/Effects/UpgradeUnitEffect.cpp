/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "UpgradeUnitEffect.h"
#include "../Units/Unit.h"

UpgradeUnitEffect::~UpgradeUnitEffect() {

}

UpgradeUnitEffect::UpgradeUnitEffect(float timeInMilliseconds, float tickEveryMilliSeconds) :
		Effect(timeInMilliseconds, tickEveryMilliSeconds) {
	_isHarmful = false;
	_GUID = 6;
	_isStackAble = false; //if false, only duration is updated
	_isForceRemoveAble = false;
}

void UpgradeUnitEffect::onApply(Unit* owner) {
}

void UpgradeUnitEffect::perform(Unit* owner) {
	owner->upgradeHitpoints();
	owner->getAttack()->upgrade();
	owner->getArmour()->upgrade();
}

void UpgradeUnitEffect::onRemoving(Unit* owner) {
}
