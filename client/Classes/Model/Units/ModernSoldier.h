/*
 * ModernSoldier.h
 *
 *  Created on: 02.06.2013
 *      Author: Planke
 */

#ifndef MODERNSOLDIER_H_
#define MODERNSOLDIER_H_

#include "MoveAbleUnit.h"

class ModernSoldier: public MoveAbleUnit {
public:
	ModernSoldier(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~ModernSoldier();
	void onDie();
	virtual Unit* clone();

private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* MODERNSOLDIER_H_ */
