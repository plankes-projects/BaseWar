/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef ARCHER_H_
#define ARCHER_H_

#include "MoveAbleUnit.h"

class Archer: public MoveAbleUnit {
public:
	Archer(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Archer();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* ARCHER_H_ */
