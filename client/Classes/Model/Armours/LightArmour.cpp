/*
 * NormalArmour.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "LightArmour.h"
#include "../../Tools/Tools.h"

LightArmour::LightArmour() {
	_damageReduction.push_back(NORMAL);
}

LightArmour::~LightArmour() {
}

Armour* LightArmour::clone() {
	Armour* clone = new LightArmour();
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

std::string LightArmour::getArmourInfo() {
	float mult = 0.5 - getArmourMultiplier();
	if (mult < 0.1)
		mult = 0.1;

	return "Normal damage taken is reduced to " +Tools::toString((int)(mult*100))+ "%.";
}

float LightArmour::realHeal(float heal, DamageType healType) {
	return heal;
}
