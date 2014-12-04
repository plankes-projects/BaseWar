/*
 * Pinkie.h
 *
 *  Created on: 24.07.2013
 *      Author: Planke
 */

#ifndef PINKIE_H_
#define PINKIE_H_

#include "MoveAbleUnit.h"

class Pinkie: public MoveAbleUnit {
public:
	Pinkie(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Pinkie();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
	virtual void fillIdleAnimationPictures();
};

#endif /* PINKIE_H_ */
