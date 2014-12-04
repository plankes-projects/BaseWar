/*
 * ArrowProjectile.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef IceProjectile_H_
#define IceProjectile_H_

#include "Projectile.h"

class Unit;

class IceProjectile: public Projectile {
public:
	IceProjectile(float slowMultiplicatorBy, float slowDuration);
	virtual ~IceProjectile();

	virtual Projectile* cloneWithDamage(float dmg);

	virtual void turnIntoUltimate() {
		//at 1 * 5 / 2 + 1 = 3 slows at master unit
		_numOfSlowedTargets = (UNITS_TO_ULTIMATE_UNIT_MULT * _numOfSlowedTargets / 2) + 1;
	}

	virtual void turnIntoMaster() {
		_numOfSlowedTargets = (UNITS_TO_MASTER_UNIT_MULT * _numOfSlowedTargets / 2) + 1;
	}

protected:
	IceProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, float slowMultiplicatorBy, float slowDuration,
			std::string info, int numOfSlowedTargets, float rangeOfSlow);
	void virtual onHit(Unit* target);
	virtual void fillMoveAnimationPictures();
	float _slowMultiplicatorBy;
	float _slowDuration;
	int _numOfSlowedTargets;
	float _rangeOfSlow;
};

#endif /* IceProjectile_H_ */
