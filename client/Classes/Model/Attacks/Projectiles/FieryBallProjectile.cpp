/*
 * FieryBallProjectile.cpp
 *
 *  Created on: 02.06.2013
 *      Author: Planke
 */

#include "FieryBallProjectile.h"
#include "../../../Sound/SoundHandler.h"

FieryBallProjectile::FieryBallProjectile(std::string image, float height, float damage, DamageType damageType, float flightSpeed, std::string info) :
		Projectile(image, height, damage, damageType, flightSpeed) {
	initAnimations();	//loading the pictures and set the animations
	_projectileInfo = info;
}

FieryBallProjectile::FieryBallProjectile() :
		Projectile("fieryBall1.png", 10, MAGIC) {
	_projectileInfo = "";
}

Projectile* FieryBallProjectile::cloneWithDamage(float dmg) {
	return new FieryBallProjectile(_image, _height, dmg, _damageType, _flightSpeed, _projectileInfo);
}

FieryBallProjectile::~FieryBallProjectile() {
	// TODO Auto-generated destructor stub
}

void FieryBallProjectile::onHit(Unit* target) {
	target->Receivedamage(_damage, _damageType);
	SoundHandler::getInstance()->playEffect("sound_hit_projectile");
}

void FieryBallProjectile::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.1;

	char str[100] = {0};
	for(int i = 1; i <= 6; ++i)
	{
	    sprintf(str, "fieryBall%d.png", i);
	    addMoveAnimationPicture(str);
	}

}
