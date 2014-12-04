/*
 * Explosion.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#include "Explosion.h"

Explosion::Explosion() {
	fillAnimationPictures();
	_opacity = 150;
	_animationSound = "sound_animnation_explosion";
}

Explosion::~Explosion() {
}

void Explosion::fillAnimationPictures() {
	_animationDelay = 0.1;

	char str[100] = { 0 };
	for (int i = 0; i <= 9; ++i) {
		sprintf(str, "fireball_hit_%04d.png", i);
		addAnimationPicture(str);
	}
}
