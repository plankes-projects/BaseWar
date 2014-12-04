/*
 * ProjectileAttack.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef PROJECTILEATTACK_H_
#define PROJECTILEATTACK_H_

#include "../Units/Unit.h"
#include "Projectiles/ProjectileFactory.h"

class ProjectileAttack: public Attack {
public:
	virtual ~ProjectileAttack();
	ProjectileAttack(ProjectileFactory* projectileFactory, float damagePerSec, float attackSpeed, float attackRange);
	virtual std::string getAttackInfo();
	virtual Attack* clone();

	virtual void turnIntoUltimate() {
		_damage *= UNITS_TO_ULTIMATE_UNIT_MULT;
		_projectileFactory->turnIntoUltimate();
	}

	virtual void turnIntoMaster() {
		_damage *= UNITS_TO_MASTER_UNIT_MULT;
		_projectileFactory->turnIntoMaster();
	}

private:
	void virtual BAM(Unit* attacker, Unit* target);
	ProjectileFactory* _projectileFactory;
};

#endif /* PROJECTILEATTACK_H_ */
