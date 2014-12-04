/*
 * Tower.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef TOWER_H_
#define TOWER_H_

#include "Unit.h"

class Tower: public Unit {
public:
	Tower();
	virtual ~Tower();

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

#endif /* TOWER_H_ */
