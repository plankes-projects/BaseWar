/*
 * LifeLeechAttack.cpp
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#include "LifeLeechAttack.h"
#include "../Units/Unit.h"
#include "../../Tools/Tools.h"

LifeLeechAttack::~LifeLeechAttack() {
	// TODO Auto-generated destructor stub
}

std::string LifeLeechAttack::getAttackInfo() {
	return "Leeching " + Tools::toString(_lifeLeech * 100) + "% life of the damage done.";
}

LifeLeechAttack::LifeLeechAttack(float lifeLeech, float damagePerSecond, DamageType damageType, float attackSpeed, float attackRange) :
		Attack(damagePerSecond, damageType, attackSpeed, attackRange) {
	_lifeLeech = lifeLeech;
}

Attack* LifeLeechAttack::clone() {
	Attack* clone = new LifeLeechAttack(_lifeLeech, getDamagePerSecond(), _damageType, _attackSpeed, _attackRange);
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

void LifeLeechAttack::BAM(Unit* attacker, Unit* target) {
	float dmg = _damage * getDamageMultiplier();

	attacker->recieveHeal(target->Receivedamage(dmg, _damageType) * _lifeLeech, _damageType);
}
