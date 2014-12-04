/*
 * NormalArmour.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "LifeRegArmour.h"
#include "../Units/Unit.h"
#include "../../Tools/BW_Time.h"
#include "../../Tools/Tools.h"

LifeRegArmour::LifeRegArmour(int hitPointsPerSeconds) {
	_hitPointsPerSeconds = hitPointsPerSeconds;
	_lastUpdate = BW_Time::getMilliSecondsCached();
}

Armour* LifeRegArmour::clone() {
	Armour* clone = new LifeRegArmour(_hitPointsPerSeconds);
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

LifeRegArmour::~LifeRegArmour() {
}

void LifeRegArmour::update(Unit* owner) {
	double span = BW_Time::getMilliSecondsCached() - _lastUpdate;
	float addLife = ((float) _hitPointsPerSeconds) / 1000.0f * span;
	float addLifeMult = getArmourMultiplier() * 3 + 1;
	owner->recieveHeal(addLife * addLifeMult, NORMAL);

	_lastUpdate = BW_Time::getMilliSecondsCached();
}

std::string LifeRegArmour::getArmourInfo() {
	return "Regenerates " + Tools::toString((int) (_hitPointsPerSeconds * getArmourMultiplier())) + "hitpoints / second";
}

float LifeRegArmour::realHeal(float heal, DamageType healType) {
	return heal;
}
