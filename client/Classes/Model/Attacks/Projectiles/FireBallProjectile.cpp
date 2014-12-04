/*
 * FireBallProjectile.cpp
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#include "FireBallProjectile.h"
#include "../../../Sound/SoundHandler.h"

FireBallProjectile::FireBallProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, std::string info) :
		Projectile(image, height, damage, damageType, flightSpeed) {
	initAnimations();	//loading the pictures and set the animations
	_projectileInfo = info;
}

FireBallProjectile::FireBallProjectile() :
		Projectile("fireball01.png", 20, MAGIC) {
	_projectileInfo = "";
}

Projectile* FireBallProjectile::cloneWithDamage(float dmg) {
	return new FireBallProjectile(_image, _height, dmg, _damageType, _flightSpeed, _projectileInfo);
}

FireBallProjectile::~FireBallProjectile() {
	// TODO Auto-generated destructor stub
}

void FireBallProjectile::onHit(Unit* target) {
	target->Receivedamage(_damage, _damageType);
	SoundHandler::getInstance()->playEffect("sound_hit_projectile");
}

void FireBallProjectile::fillMoveAnimationPictures() {
}
