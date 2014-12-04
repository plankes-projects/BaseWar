/*
 * NormalProjectile.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef NORMALPROJECTILE_H_
#define NORMALPROJECTILE_H_

#include "../../Units/Unit.h"
#include "Projectile.h"

class NormalProjectile : public Projectile{
public:
	NormalProjectile(bool chaosDamage = false);
	virtual ~NormalProjectile();
	virtual Projectile* cloneWithDamage(float dmg);
protected:
	NormalProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, std::string info);
	void virtual onHit(Unit* target);
	virtual void fillMoveAnimationPictures();
};

#endif /* NORMALPROJECTILE_H_ */
