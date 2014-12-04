/*
 * Darksaber.h
 *
 *  Created on: 18.06.2013
 *      Author: Planke
 */

#ifndef DARKSABER_H_
#define DARKSABER_H_

#include "MoveAbleUnit.h"

class Darksaber: public MoveAbleUnit {
public:
	Darksaber(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Darksaber();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* DARKSABER_H_ */
