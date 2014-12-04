/*
 * ProjectileFactory.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "ProjectileFactory.h"

Projectile* ProjectileFactory::createProjectile(float damage) {
	return _projectile->cloneWithDamage(damage);
}

std::string ProjectileFactory::getprojectileInfo() {
	return _projectile->getProjectileInfo();
}

ProjectileFactory::ProjectileFactory(Projectile* p) {
	_projectile = p;
}

ProjectileFactory* ProjectileFactory::clone() {
	return new ProjectileFactory(_projectile->cloneWithDamage(0));
}

ProjectileFactory::~ProjectileFactory() {
	delete _projectile;
}

DamageType ProjectileFactory::getDamageType() {
	return _projectile->getDamageType();
}
