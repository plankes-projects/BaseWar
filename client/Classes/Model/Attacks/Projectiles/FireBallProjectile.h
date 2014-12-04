/*
 * FireBallProjectile.h
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#ifndef FIREBALLPROJECTILE_H_
#define FIREBALLPROJECTILE_H_

#include "../../Units/Unit.h"
#include "Projectile.h"

class FireBallProjectile: public Projectile {
public:
	FireBallProjectile();
	virtual ~FireBallProjectile();
	virtual Projectile* cloneWithDamage(float dmg);
protected:
	FireBallProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, std::string info);
	void virtual onHit(Unit* target);
	virtual void fillMoveAnimationPictures();
};

#endif /* FIREBALLPROJECTILE_H_ */
