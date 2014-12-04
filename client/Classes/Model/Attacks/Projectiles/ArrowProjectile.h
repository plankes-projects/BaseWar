/*
 * ArrowProjectile.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef ARROWPROJECTILE_H_
#define ARROWPROJECTILE_H_

#include "Projectile.h"

class Unit;

class ArrowProjectile : public Projectile{
public:
	ArrowProjectile();
	virtual ~ArrowProjectile();

	virtual Projectile* cloneWithDamage(float dmg);
protected:
	ArrowProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, std::string info);
	void virtual onHit(Unit* target);
	virtual void fillMoveAnimationPictures();
};

#endif /* ARROWPROJECTILE_H_ */
