/*
 * NormalArmour.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "HeavyArmour.h"
#include "../../Tools/Tools.h"

HeavyArmour::HeavyArmour() {
	_damageReduction.push_back(NORMAL);
	_damageReduction.push_back(PIERCING);
}

HeavyArmour::~HeavyArmour() {
}

Armour* HeavyArmour::clone() {
	Armour* clone = new HeavyArmour();
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

std::string HeavyArmour::getArmourInfo() {
	float mult = 0.5 - getArmourMultiplier();
	if (mult < 0.1)
		mult = 0.1;

	return "Normal/Piercing damage taken is reduced to " +Tools::toString((int)(mult*100))+ "%.";
}

float HeavyArmour::realHeal(float heal, DamageType healType) {
	return heal;
}
