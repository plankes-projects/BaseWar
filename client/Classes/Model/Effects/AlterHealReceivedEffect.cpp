/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "AlterHealReceivedEffect.h"
#include "../Units/Unit.h"

AlterHealReceivedEffect::~AlterHealReceivedEffect() {

}

AlterHealReceivedEffect::AlterHealReceivedEffect(float timeInMilliseconds, float percent, bool isHarmful) :
		Effect(timeInMilliseconds, NOTICK) {
	_isHarmful = isHarmful;
	_GUID = 7;
	_isStackAble = true;
	_isForceRemoveAble = true;
	_percent = percent;
}

void AlterHealReceivedEffect::onApply(Unit* owner) {
	owner->multiplayReceivedHealBy(_percent);
}

void AlterHealReceivedEffect::perform(Unit* owner) {
}

void AlterHealReceivedEffect::onRemoving(Unit* owner) {
	owner->devideReceivedHealBy(_percent);
}
