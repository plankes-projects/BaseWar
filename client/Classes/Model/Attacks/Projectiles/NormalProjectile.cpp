/*
 * NormalProjectile.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "NormalProjectile.h"
#include "../../../Sound/SoundHandler.h"

NormalProjectile::NormalProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, std::string info) :
		Projectile(image, height, damage, damageType, flightSpeed) {
	initAnimations();	//loading the pictures and set the animations
	_projectileInfo = info;
}

NormalProjectile::NormalProjectile(bool chaosDamage) :
		Projectile("missleD0001.png", 10, chaosDamage ? CHAOS : PIERCING) {
	_projectileInfo = "";
}

Projectile* NormalProjectile::cloneWithDamage(float dmg) {
	return new NormalProjectile(_image, _height, dmg, _damageType, _flightSpeed, _projectileInfo);
}

NormalProjectile::~NormalProjectile() {
	// TODO Auto-generated destructor stub
}

void NormalProjectile::onHit(Unit* target) {
	target->Receivedamage(_damage, _damageType);
	SoundHandler::getInstance()->playEffect("sound_hit_projectile");
}

void NormalProjectile::fillMoveAnimationPictures() {
}
