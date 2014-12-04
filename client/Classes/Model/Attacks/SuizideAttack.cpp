/*
 * LifeLeechAttack.cpp
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#include "SuizideAttack.h"
#include "../Units/Unit.h"
#include "../../Tools/Tools.h"
#include "../../Model/Model.h"
#include "../../View/Animations/Explosion.h"

SuizideAttack::~SuizideAttack() {
	// TODO Auto-generated destructor stub
}

std::string SuizideAttack::getAttackInfo() {
	return "Explodes on contact. Hitting " + Tools::toString(_numberOfHitTargets) + " targets.";
}

SuizideAttack::SuizideAttack(float splashRange, float damage) :
		Attack(damage, MAGIC, 1, 50) {
	_splashRange = splashRange;
	_numberOfHitTargets = 5;
}


Attack* SuizideAttack::clone(){
	Attack* clone = new SuizideAttack(_splashRange, _damage);
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

void SuizideAttack::BAM(Unit* attacker, Unit* target) {
	(new Explosion())->animateAt(attacker->getParent(), attacker->getPositionBW(), _splashRange * 2, true);

	attacker->kill();
	float dmg = _damage * getDamageMultiplier();

	//do the dmg
	int hitTargets = 0;
	std::list<Unit*> enemyUnits = Model::getInstance()->getMyArmy(target->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {
		if (hitTargets >= _numberOfHitTargets)
			break;

		if ((*it)->getHitpoints() > 0 && attacker->distanceTo((*it)) <= _splashRange) {
			hitTargets++;
			(*it)->Receivedamage(dmg, _damageType);
		}
	}
}
