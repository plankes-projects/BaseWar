/*
 * Troll.h
 *
 *  Created on: 29.05.2013
 *      Author: Planke
 */

#ifndef TROLL_H_
#define TROLL_H_

#include "MoveAbleUnit.h"

class Troll: public MoveAbleUnit {
public:
	Troll(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Troll();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* TROLL_H_ */
