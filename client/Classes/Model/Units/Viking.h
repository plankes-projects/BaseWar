/*
 * Viking.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef VIKING_H_
#define VIKING_H_

#include "MoveAbleUnit.h"

class Viking: public MoveAbleUnit {
public:
	Viking(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Viking();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* VIKING_H_ */

