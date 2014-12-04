/*
 * ProjectileFactory.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef PROJECTILEFACTORY_H_
#define PROJECTILEFACTORY_H_

#include "../../ArmyType.h"
#include "Projectile.h"

class Unit;

class ProjectileFactory {
public:
	ProjectileFactory(Projectile* p);
	virtual ~ProjectileFactory();
	virtual Projectile* createProjectile(float damage);

	std::string getprojectileInfo();

	DamageType getDamageType();
	ProjectileFactory* clone();

	virtual void turnIntoUltimate() {
		_projectile->turnIntoUltimate();
	}

	virtual void turnIntoMaster() {
		_projectile->turnIntoMaster();
	}

protected:
	Projectile* _projectile;
};

#endif /* PROJECTILEFACTORY_H_ */
