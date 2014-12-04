/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "CleaveAttack.h"
#include "../Units/Unit.h"
#include "../../Tools/Tools.h"
#include "../Model.h"

CleaveAttack::~CleaveAttack() {
	// TODO Auto-generated destructor stub
}

std::string CleaveAttack::getAttackInfo() {
	return "Hitting " + Tools::toString(_numberOfAdditionalHitTargets) + " additional targets with "
			+ Tools::toString(_splashDamageInPercent * 100) + "% damage each.";
}

CleaveAttack::CleaveAttack(float splashRange, int numberOfAdditionalHitTargets, float splashDamageInPercent, float damagePerSecond,
		DamageType damageType, float attackSpeed, float attackRange) :
		Attack(damagePerSecond, damageType, attackSpeed, attackRange) {
	_splashRange = splashRange;
	_numberOfAdditionalHitTargets = numberOfAdditionalHitTargets;
	_splashDamageInPercent = splashDamageInPercent;
}

Attack* CleaveAttack::clone() {
	Attack* clone = new CleaveAttack(_splashRange, _numberOfAdditionalHitTargets, _splashDamageInPercent, getDamagePerSecond(), _damageType,
			_attackSpeed, _attackRange);
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

void CleaveAttack::BAM(Unit* attacker, Unit* target) {

	float dmg = _damage * getDamageMultiplier();
	target->Receivedamage(dmg, _damageType);

	dmg *= _splashDamageInPercent;

	int hitTargets = 0;

	std::list<Unit*> enemyUnits = Model::getInstance()->getMyArmy(target->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {
		if (hitTargets >= _numberOfAdditionalHitTargets)
			break;

		if ((*it) != target && (*it)->getHitpoints() > 0 && target->distanceTo((*it)) <= _splashRange) {
			(*it)->Receivedamage(dmg, _damageType);
			hitTargets++;
		}
	}
}
