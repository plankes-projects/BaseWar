/*
 * Projectile.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "Projectile.h"
#include "../../Model.h"
#include "../../../Tools/BW_Rect.h"

Projectile::Projectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed) :
		BW_Sprite(image, height, false, true) {
	this->_flightSpeed = flightSpeed;
	this->_damage = damage;
	this->_damageType = damageType;
	setOpacity(0);
}

Projectile::Projectile(std::string image, float height, DamageType damageType, float flightSpeed) :
		BW_Sprite(image, height, false) {
	this->_flightSpeed = flightSpeed;
	this->_damage = 0;
	this->_damageType = damageType;
}

Projectile::~Projectile() {
}

void Projectile::initAnimationVars() {

}

void Projectile::fillIdleAnimationPictures() {
	//nothing to do, this has no idle animation
}

void Projectile::fillAttackAnimationPictures() {
	//nothing to do, this has no attack animation
}

void Projectile::update() {
	Unit* target = checkForHit();
	if (target == NULL) {
		if (this->isActionsFinished())
			remove();
		return;
	}

	onHit(target);
	remove();
}

Unit* Projectile::checkForHit() {

	std::list<Unit*> enemyUnits = Model::getInstance()->getEnemyArmy(_armyType)->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {
		if ((*it)->getHitpoints() > 0 && this->hits(*it)) {
			// hit something
			return (*it);
		}
	}

	return NULL;
}

void Projectile::remove() {
	Model::getInstance()->addToRemoveProjectile(this);
}

void Projectile::setArmyType(ArmyType armyType) {
	_armyType = armyType;
}

ArmyType Projectile::getArmyType() {
	return _armyType;
}

void Projectile::flyTo(BW_Point p) {
	this->spriteMoveTo(p, _flightSpeed, true);
}
