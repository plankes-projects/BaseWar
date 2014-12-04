/*
 * Explosion.h
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "BW_Animation.h"

class Explosion: public BW_Animation {
public:
	Explosion();
	virtual ~Explosion();
	virtual void fillAnimationPictures();
};

#endif /* EXPLOSION_H_ */
