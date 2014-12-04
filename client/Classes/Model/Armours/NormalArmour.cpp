/*
 * NormalArmour.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "NormalArmour.h"
#include "../../Tools/Tools.h"

NormalArmour::NormalArmour() {
	_damageReduction.push_back(MAGIC);
	_damageReduction.push_back(PIERCING);
}

NormalArmour::~NormalArmour() {
}

Armour* NormalArmour::clone() {
	Armour* clone = new NormalArmour();
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

std::string NormalArmour::getArmourInfo() {
	float mult = 0.5 - getArmourMultiplier();
	if (mult < 0.1)
		mult = 0.1;

	return " Magic/Piercing damage taken is reduced to " +Tools::toString((int)(mult*100))+ "%.";
}

float NormalArmour::realHeal(float heal, DamageType healType) {
	return heal;
}
