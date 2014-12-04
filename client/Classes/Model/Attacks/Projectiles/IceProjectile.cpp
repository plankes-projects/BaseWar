/*
 * ArrowProjectile.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "IceProjectile.h"
#include "../../../Tools/Tools.h"
#include "../../Effects/IceEffect.h"
#include "../../Model.h"

IceProjectile::IceProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, float slowMultiplicatorBy,
		float slowDuration, std::string info, int numOfSlowedTargets, float rangeOfSlow) :
		Projectile(image, height, damage, damageType, flightSpeed) {
	initAnimations(); //loading the pictures and set the animations
	_slowMultiplicatorBy = slowMultiplicatorBy;
	_slowDuration = slowDuration;
	_projectileInfo = info;
	_numOfSlowedTargets = numOfSlowedTargets;
	_rangeOfSlow = rangeOfSlow;
}

IceProjectile::IceProjectile(float slowMultiplicatorBy, float slowDuration) :
		Projectile("iceProjectile.png", 20, MAGIC) {
	_slowMultiplicatorBy = slowMultiplicatorBy;
	_slowDuration = slowDuration;
	_projectileInfo = " - Slowing enemy by " + Tools::toString((int) (_slowMultiplicatorBy * 100)) + "% for " + Tools::toString((int) (_slowDuration / 1000))
			+ " sec.";

	_numOfSlowedTargets = 1;
	_rangeOfSlow = 200;
}

Projectile* IceProjectile::cloneWithDamage(float dmg) {
	return new IceProjectile(_image, _height, dmg, _damageType, _flightSpeed, _slowMultiplicatorBy, _slowDuration, _projectileInfo, _numOfSlowedTargets,
			_rangeOfSlow);
}

IceProjectile::~IceProjectile() {
}

void IceProjectile::onHit(Unit* target) {
	target->Receivedamage(_damage, _damageType);

	int numOfSlow = _numOfSlowedTargets;
	if (numOfSlow > 0) {
		target->applyEffect(new IceEffect(_slowDuration, _slowMultiplicatorBy, _slowMultiplicatorBy));
		numOfSlow--;
	}

	//case of normal frost mage
	if (numOfSlow == 0)
		return;

	//slow rest units
	std::list<Unit*> enemyUnits = Model::getInstance()->getMyArmy(target->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {
		if (numOfSlow == 0)
			break;

		if ((*it) != target && (*it)->getHitpoints() > 0 && target->distanceTo((*it)) <= _rangeOfSlow) {
			numOfSlow--;
			(*it)->applyEffect(new IceEffect(_slowDuration, _slowMultiplicatorBy, _slowMultiplicatorBy));
		}
	}

}

void IceProjectile::fillMoveAnimationPictures() {

}
