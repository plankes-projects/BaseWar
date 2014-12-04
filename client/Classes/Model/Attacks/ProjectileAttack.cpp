/*
 * ProjectileAttack.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "ProjectileAttack.h"
#include "../Model.h"

ProjectileAttack::~ProjectileAttack() {
	delete _projectileFactory;
}

std::string ProjectileAttack::getAttackInfo() {
	//_projectileFactory
	return "Range" + _projectileFactory->getprojectileInfo();
}

ProjectileAttack::ProjectileAttack(ProjectileFactory* projectileFactory, float damagePerSec, float attackSpeed, float attackRange) :
		Attack(damagePerSec, projectileFactory->getDamageType(), attackSpeed, attackRange) {

	_projectileFactory = projectileFactory;
}

Attack* ProjectileAttack::clone() {
	Attack* clone = new ProjectileAttack(_projectileFactory->clone(), getDamagePerSecond(), _attackSpeed, _attackRange);
	clone->cloneUpgradeStatsFrom(this);
	return clone;
}

void ProjectileAttack::BAM(Unit* attacker, Unit* target) {
	//shot projectile
	Projectile* projectile = _projectileFactory->createProjectile(_damage * getDamageMultiplier());
	projectile->setPosition(attacker->getPositionBW());
	projectile->flyTo(target->getPositionBW());
	Model::getInstance()->getMyArmy(_armyType)->addProjectile(projectile);
}
