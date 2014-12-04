/*
 * Baron.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef BARON_H_
#define BARON_H_

#include "MoveAbleUnit.h"

class Baron: public MoveAbleUnit {
public:
	Baron(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Baron();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* BARON_H_ */
