/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "Effect.h"
#include "../../Tools/BW_Time.h"
#include "../Units/Unit.h"
#include "../Model.h"

Effect::~Effect() {
}

const float Effect::FOREVER = -1;
const float Effect::NOTICK = -1;

Effect::Effect(float timeInMilliseconds, float tickEveryMilliSeconds) {
	_lastperform = BW_Time::getMilliSecondsCached();
	_firstPerform = true;
	_tickDelay = tickEveryMilliSeconds;
	_forceRemove = false;
	_isForceRemoveAble = true;
	_duration = timeInMilliseconds;

	_removeTime = timeInMilliseconds == FOREVER ? FOREVER : _lastperform + (double) timeInMilliseconds;

	_GUID = 0; //should be overwritten by subclasses
	_isStackAble = true; //if false, only duration is updated
}

void Effect::reApply(Effect* effect) {
	float newDuration = effect->getDuration();
	double now = BW_Time::getMilliSecondsCached();
	if (now + newDuration > _removeTime) {
		_removeTime = now + newDuration;
	}
}

bool Effect::forceRemove() {
	if (_forceRemove || !_isForceRemoveAble)
		return false;

	_forceRemove = true;
	return true;
}

bool Effect::tryPerform(Unit* owner) {
	if (_firstPerform) {
		onApply(owner);
		_firstPerform = false;
	}

	//if force remove we do not want the effect to get another tick
	if (_forceRemove) {
		//remove me
		onRemoving(owner);
		return false;
	}

	double now = BW_Time::getMilliSecondsCached();
	if (_tickDelay != NOTICK && _lastperform < now - _tickDelay) {
		perform(owner);
		_lastperform += _tickDelay;
	}

	//if timeover we want to give the effect the chance for a last tick
	if (_removeTime != FOREVER && now > _removeTime) {
		//remove me
		onRemoving(owner);
		return false;
	}
	return true;
}
