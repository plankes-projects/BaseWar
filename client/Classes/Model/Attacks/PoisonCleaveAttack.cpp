/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "PoisonCleaveAttack.h"
#include "../Units/Unit.h"
#include "../../Tools/Tools.h"
#include "../Model.h"
#include "../Effects/PoisonEffect.h"

PoisonCleaveAttack::~PoisonCleaveAttack() {
	// TODO Auto-generated destructor stub
}

std::string PoisonCleaveAttack::getAttackInfo() {
	return "Hitting " + Tools::toString(_numberOfHitTargets) + " targets with damage over " + Tools::toString((int) _poisonTime / 1000)
			+ " sec.";
}

PoisonCleaveAttack::PoisonCleaveAttack(float poisonTime, float splashRange, int numberOfHitTargets, float damagePerSecond,
		DamageType damageType, float attackSpeed, float attackRange) :
		Attack(damagePerSecond, damageType, attackSpeed, attackRange) {
	_splashRange = splashRange;
	_numberOfHitTargets = numberOfHitTargets;
	_poisonTime = poisonTime;
}

Attack* PoisonCleaveAttack::clone() {
	Attack* clone = new PoisonCleaveAttack(_poisonTime, _splashRange, _numberOfHitTargets, getDamagePerSecond(), _damageType, _attackSpeed,
			_attackRange);
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

void PoisonCleaveAttack::BAM(Unit* attacker, Unit* target) {

	float dmg = _damage * getDamageMultiplier();
	float tickEvery = 500;
	int hitTargets = 1; //1 because we already hit primary the target

	float tickDmg = dmg / (_poisonTime / tickEvery);

	target->applyEffect(new PoisonEffect(_poisonTime, tickEvery, tickDmg));

	std::list<Unit*> enemyUnits = Model::getInstance()->getMyArmy(target->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {
		if (hitTargets >= _numberOfHitTargets)
			break;

		if ((*it) != target && (*it)->getHitpoints() > 0 && target->distanceTo((*it)) <= _splashRange) {
			(*it)->applyEffect(new PoisonEffect(_poisonTime, tickEvery, tickDmg));
			hitTargets++;
		}
	}
}
