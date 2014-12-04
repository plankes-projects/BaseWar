/*
 * Minion.h
 *
 *  Created on: 24.07.2013
 *      Author: Planke
 */

#ifndef MINION_H_
#define MINION_H_

#include "MoveAbleUnit.h"

class Minion: public MoveAbleUnit {
public:
	Minion(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Minion();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* MINION_H_ */
