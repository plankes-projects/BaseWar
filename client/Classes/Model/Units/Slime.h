/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef Slime_H_
#define Slime_H_

#include "MoveAbleUnit.h"

class Slime: public MoveAbleUnit {
public:
	Slime(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Slime();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* ARCHER_H_ */
