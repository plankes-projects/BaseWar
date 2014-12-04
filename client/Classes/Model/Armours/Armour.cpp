/*
 * Armour.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "Armour.h"
#include "../Units/Unit.h"

Armour::Armour() {
	_upgradeNum = 0;
	_upgradeMultiplier = 0.05; //standard value

	_armourEffectMultiplier = 1;
}

Armour::~Armour() {
}

void Armour::cloneUpgradeStatsFrom(Armour* armour) {
	_upgradeMultiplier = armour->getUpgrademultiplier();
	_upgradeNum = armour->getUpgradeNum();
}

void Armour::upgrade() {
	_upgradeNum++;
}

void Armour::update(Unit* owner) {

}

float Armour::getArmourMultiplier() {
	return _upgradeMultiplier * _upgradeNum * _armourEffectMultiplier;
}

float Armour::realDamage(float damage, DamageType damageType) {
	if(std::find(_damageReduction.begin(), _damageReduction.end(), damageType) != _damageReduction.end()) {
	    /* v contains x */
		float mult = 0.5 - getArmourMultiplier();
		if (mult < 0.1)
			mult = 0.1;
		return damage * mult;
	} else {
	    /* v does not contain x */
		return damage;
	}
}
