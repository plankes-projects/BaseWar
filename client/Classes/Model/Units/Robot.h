/*
 * Robot.h
 *
 *  Created on: 24.07.2013
 *      Author: Planke
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include "MoveAbleUnit.h"

class Robot: public MoveAbleUnit {
public:
	Robot(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~Robot();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* ROBOT_H_ */

