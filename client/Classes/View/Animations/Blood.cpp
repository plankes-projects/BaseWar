/*
 * Blood.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#include "Blood.h"

Blood::Blood() {
	fillAnimationPictures();
}

Blood::~Blood() {
}

void Blood::fillAnimationPictures() {
	_animationDelay = 0.1;

	char str[100] = { 0 };
	for (int i = 1; i <= 6; ++i) {
		sprintf(str, "blood_d_%04d.png", i);
		addAnimationPicture(str);
	}
}
