/*
 * Explosion.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#include "SmokeBomb.h"

SmokeBomb::SmokeBomb() {
	fillAnimationPictures();
	_opacity = 150;
	_animationSound = "sound_animnation_smokebomb";
}

SmokeBomb::~SmokeBomb() {
}

void SmokeBomb::fillAnimationPictures() {
	_animationDelay = 0.05;

	char str[100] = { 0 };
	for (int i = 0; i <= 15; ++i) {
		sprintf(str, "smoke_up%04d.png", i);
		addAnimationPicture(str);
	}
}
