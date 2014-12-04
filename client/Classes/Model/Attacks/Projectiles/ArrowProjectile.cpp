/*
 * ArrowProjectile.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "ArrowProjectile.h"
#include "../../../Sound/SoundHandler.h"

ArrowProjectile::ArrowProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, std::string info) :
		Projectile(image, height, damage, damageType, flightSpeed) {
	initAnimations(); //loading the pictures and set the animations
	_projectileInfo = info;
}

ArrowProjectile::ArrowProjectile() :
		Projectile("arrow.png", 4, PIERCING) {
	_projectileInfo = "";
}

Projectile* ArrowProjectile::cloneWithDamage(float dmg) {
	return new ArrowProjectile(_image, _height, dmg, _damageType, _flightSpeed, _projectileInfo);
}

ArrowProjectile::~ArrowProjectile() {
	// TODO Auto-generated destructor stub
}

void ArrowProjectile::onHit(Unit* target) {
	target->Receivedamage(_damage, _damageType);
	SoundHandler::getInstance()->playEffect("sound_hit_projectile");

}

void ArrowProjectile::fillMoveAnimationPictures() {

}
