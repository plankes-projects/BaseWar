/*
 * NinjaRange.h
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#ifndef NINJARANGE_H_
#define NINJARANGE_H_

#include "MoveAbleUnit.h"

class NinjaRange: public MoveAbleUnit {
public:
	NinjaRange(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~NinjaRange();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* NINJARANGE_H_ */
