/*
 * Base.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef BASE_H_
#define BASE_H_

#include "Unit.h"

class Base: public Unit {
public:
	Base();
	virtual ~Base();

	void onDie();
	virtual Unit* clone();
	virtual bool isImmuneToEffect(Effect* effect) {
		return true;
	}
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillIdleAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* BASE_H_ */
