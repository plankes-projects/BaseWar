/*
 * MoveAbleUnit.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef MOVEABLEUNIT_H_
#define MOVEABLEUNIT_H_

#include "Unit.h"

class MoveAbleUnit: public Unit {
public:
	MoveAbleUnit(int addCost, std::string image, float height, float runspeed, float hitpoints, Attack* attack, Armour* armour,
			std::list<Skill*> skills, float mana, float manaRegPerSeconds);
	virtual ~MoveAbleUnit();
	virtual void moveTo(BW_Point location);
	virtual bool isMoveAble();

private:
	virtual void fillIdleAnimationPictures(); //we do not need idle at moveable units
};

#endif /* MOVEABLEUNIT_H_ */
