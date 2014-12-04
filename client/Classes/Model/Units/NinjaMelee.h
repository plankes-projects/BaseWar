/*
 * NinjaMelee.h
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#ifndef NINJAMELEE_H_
#define NINJAMELEE_H_

#include "MoveAbleUnit.h"

class NinjaMelee: public MoveAbleUnit {
public:
	NinjaMelee(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~NinjaMelee();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* NINJAMELEE_H_ */

