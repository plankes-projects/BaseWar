/*
 * NormalArmour.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "UnarmouredArmour.h"
#include "../../Tools/Tools.h"


UnarmouredArmour::UnarmouredArmour() {
	_damageReduction.push_back(MAGIC);
}

UnarmouredArmour::~UnarmouredArmour() {
}

Armour* UnarmouredArmour::clone() {
	Armour* clone = new UnarmouredArmour();
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

std::string UnarmouredArmour::getArmourInfo() {
	float mult = 0.5 - getArmourMultiplier();
	if (mult < 0.1)
		mult = 0.1;

	return "Magic damage taken is reduced to " +Tools::toString((int)(mult*100))+ "%.";
}

float UnarmouredArmour::realHeal(float heal, DamageType healType) {
	return heal;
}
