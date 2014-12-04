/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef FlareSkelet_H_
#define FlareSkelet_H_

#include "MoveAbleUnit.h"

class FlareSkelet: public MoveAbleUnit {
public:
	FlareSkelet(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~FlareSkelet();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* ARCHER_H_ */
