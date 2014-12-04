/*
 * InstantHeal.cpp
 *
 *  Created on: 28.05.2013
 *      Author: Planke
 */

#include "InstantHeal.h"
#include "../Units/Unit.h"

InstantHeal::InstantHeal(float healPerSecond, DamageType healType, float healSpeed, float healRange) :
		Attack(healPerSecond, healType, healSpeed, healRange) {
	_isHeal = true;
}

Attack* InstantHeal::clone() {
	Attack* clone = new InstantHeal(getDamagePerSecond(), _damageType, _attackSpeed, _attackRange);
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

std::string InstantHeal::getAttackInfo() {
	return "Normal";
}

InstantHeal::~InstantHeal() {
}

void InstantHeal::BAM(Unit* attacker, Unit* target) {
	float dmg = _damage * getDamageMultiplier();
	target->recieveHeal(dmg, _damageType); //+10% per upgrade
}

