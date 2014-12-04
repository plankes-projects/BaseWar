/*
 * RangeAttack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "InstantAttack.h"
#include "../Units/Unit.h"

InstantAttack::~InstantAttack() {
	// TODO Auto-generated destructor stub
}

std::string InstantAttack::getAttackInfo() {
	return "Normal";
}

InstantAttack::InstantAttack(float damagePerSecond, DamageType damageType, float attackSpeed, float attackRange) :
		Attack(damagePerSecond, damageType, attackSpeed, attackRange) {
}

Attack* InstantAttack::clone() {
	Attack* clone = new InstantAttack(getDamagePerSecond(), _damageType, _attackSpeed, _attackRange);
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

void InstantAttack::BAM(Unit* attacker, Unit* target) {
	float dmg = _damage * getDamageMultiplier();
	target->Receivedamage(dmg, _damageType); //+10% per upgrade
}

