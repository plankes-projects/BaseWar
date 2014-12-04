/*
 * Projectile.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "../../DamageType.h"
#include "../../../View/BW_Sprite.h"
#include "../../../Model/Units/Unit.h"
#include "../../../Constants.h"

class Projectile: public BW_Sprite {
public:
	Projectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed = PROJECTILE_SPEED);
	Projectile(std::string image, float height, DamageType damageTyp, float flightSpeed = PROJECTILE_SPEED);
	virtual ~Projectile();

	virtual std::string getProjectileInfo() {
		return _projectileInfo;
	}

	void update();
	void remove();
	void setArmyType(ArmyType armyType);
	ArmyType getArmyType();
	void flyTo(BW_Point p);

	virtual Projectile* cloneWithDamage(float dmg) = 0;

	DamageType getDamageType() {
		return _damageType;
	}

	virtual void turnIntoUltimate(){
	}

	virtual void turnIntoMaster(){
	}

protected:
	std::string _projectileInfo;
	float _flightSpeed;
	ArmyType _armyType; // is set by Army::addProjectile()
	float _damage;
	DamageType _damageType;
	Unit* checkForHit();
	void virtual onHit(Unit* target) = 0;

private:
	void fillIdleAnimationPictures(); //we do not need idle at projectile
	void fillAttackAnimationPictures();
	virtual void initAnimationVars();
};

#endif /* PROJECTILE_H_ */
