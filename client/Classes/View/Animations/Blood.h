/*
 * Blood.h
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#ifndef BLOOD_H_
#define BLOOD_H_

#include "BW_Animation.h"

class Blood: public BW_Animation {
public:
	Blood();
	virtual ~Blood();
	virtual void fillAnimationPictures();
};

#endif /* BLOOD_H_ */
