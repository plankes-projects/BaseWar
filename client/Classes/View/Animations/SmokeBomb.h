/*
 * Explosion.h
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "BW_Animation.h"

class SmokeBomb: public BW_Animation {
public:
	SmokeBomb();
	virtual ~SmokeBomb();
	virtual void fillAnimationPictures();
};

#endif /* EXPLOSION_H_ */
