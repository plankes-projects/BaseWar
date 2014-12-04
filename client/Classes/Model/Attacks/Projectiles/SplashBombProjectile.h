/*
 * SplashBombProjectile.h
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#ifndef SPLASHBOMBPROJECTILE_H_
#define SPLASHBOMBPROJECTILE_H_

#include "../../Units/Unit.h"
#include "Projectile.h"

class SplashBombProjectile : public Projectile{
public:
	SplashBombProjectile(float splashRange, float splashDamageInPercent, bool chaosDamage = false);
	virtual ~SplashBombProjectile();
	virtual Projectile* cloneWithDamage(float dmg);
protected:
	SplashBombProjectile(int numHits, float splashRange, float splashDamageInPercent, std::string image, float height, float damage, DamageType damageType, float flightSpeed, std::string info);
	void virtual onHit(Unit* target);
	virtual void fillMoveAnimationPictures();
	float _splashRange;
	int _numberOfHitTargets;
	float _splashDamageInPercent;
};
#endif /* SPLASHBOMBPROJECTILE_H_ */
