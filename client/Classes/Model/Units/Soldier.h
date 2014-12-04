/*
 * Soldier.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef Soldier_H_
#define Soldier_H_

#include "MoveAbleUnit.h"

class Soldier: public MoveAbleUnit {
public:
	Soldier(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Soldier();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* Soldier_H_ */
