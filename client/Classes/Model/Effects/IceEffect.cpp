/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "IceEffect.h"
#include "../Units/Unit.h"
#include "../../Sound/SoundHandler.h"

IceEffect::~IceEffect() {

}

IceEffect::IceEffect(float timeInMilliseconds, float decreasedAttackSpeedInPercent,
		float decreasedRunSpeedInPercent) :
		Effect(timeInMilliseconds, NOTICK) {
	_decreasedAttackSpeedInPercent = decreasedAttackSpeedInPercent;
	_decreasedRunSpeedInPercent = decreasedRunSpeedInPercent;
	_isHarmful = true;
	_GUID = 3;
	_isStackAble = false; //if false, only duration is updated
}

void IceEffect::onApply(Unit* owner) {
	owner->addBlueColor();
	owner->decreaseAttackSpeedBy(_decreasedAttackSpeedInPercent);
	owner->decreaseRunSpeedBy(_decreasedRunSpeedInPercent);
	SoundHandler::getInstance()->playEffect("sound_hit_projectile");
}

void IceEffect::perform(Unit* owner) {
	//nothing to do
}

void IceEffect::onRemoving(Unit* owner) {
	owner->removeBlueColor();
	owner->increaseAttackSpeedBy(_decreasedAttackSpeedInPercent);
	owner->increaseRunSpeedBy(_decreasedRunSpeedInPercent);
}
