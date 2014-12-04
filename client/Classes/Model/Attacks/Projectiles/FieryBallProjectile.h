/*
 * FieryBallProjectile.h
 *
 *  Created on: 02.06.2013
 *      Author: Planke
 */

#ifndef FIERYBALLPROJECTILE_H_
#define FIERYBALLPROJECTILE_H_

#include "../../Units/Unit.h"
#include "Projectile.h"

class FieryBallProjectile: public Projectile {
public:
	FieryBallProjectile();
	virtual ~FieryBallProjectile();
	virtual Projectile* cloneWithDamage(float dmg);
protected:
	FieryBallProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, std::string info);
	void virtual onHit(Unit* target);
	virtual void fillMoveAnimationPictures();
};

#endif /* FIERYBALLPROJECTILE_H_ */
