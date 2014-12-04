/*
 * SplashBombProjectile.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#include "SplashBombProjectile.h"
#include "../../Model.h"
#include "../../../Tools/Tools.h"
#include "../../../View/Animations/Explosion.h"

SplashBombProjectile::SplashBombProjectile(int numHits, float splashRange, float splashDamageInPercent, std::string image, float height,
		float damage, DamageType damageType, float flightSpeed, std::string info) :
		Projectile(image, height, damage, damageType) {
	_splashRange = splashRange;
	_numberOfHitTargets = numHits;
	_splashDamageInPercent = splashDamageInPercent;
	initAnimations(); //loading the pictures and set the animations
	_projectileInfo = info;
}

SplashBombProjectile::SplashBombProjectile(float splashRange, float splashDamageInPercent, bool chaosDamage) :
		Projectile("missleC0001.png", 15, chaosDamage ? CHAOS : MAGIC) {
	_numberOfHitTargets = 4;
	_splashRange = splashRange;
	_splashDamageInPercent = splashDamageInPercent;
	_projectileInfo = " - Hitting " + Tools::toString(_numberOfHitTargets) + " additional targets with "
			+ Tools::toString(_splashDamageInPercent * 100) + "% damage each.";
}

Projectile* SplashBombProjectile::cloneWithDamage(float dmg) {
	return new SplashBombProjectile(_numberOfHitTargets, _splashRange, _splashDamageInPercent, _image, _height, dmg, _damageType,
			_flightSpeed, _projectileInfo);
}

SplashBombProjectile::~SplashBombProjectile() {
}

void SplashBombProjectile::onHit(Unit* target) {
	//place the explosion
	(new Explosion())->animateAt(getParent(), target->getPositionBW(), _splashRange * 2, true);

	float dmg = _damage;
	target->Receivedamage(dmg, _damageType);

	//do the dmg
	dmg *= _splashDamageInPercent;
	int hitTargets = 0;
	std::list<Unit*> enemyUnits = Model::getInstance()->getMyArmy(target->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {
		if (hitTargets >= _numberOfHitTargets)
			break;

		if ((*it) != target && (*it)->getHitpoints() > 0 && target->distanceTo((*it)) <= _splashRange) {
			hitTargets++;
			(*it)->Receivedamage(dmg, _damageType);
		}
	}
}

void SplashBombProjectile::fillMoveAnimationPictures() {
}
