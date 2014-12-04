/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef BotanicPriest_H_
#define BotanicPriest_H_

#include "MoveAbleUnit.h"

class BotanicPriest: public MoveAbleUnit {
public:
	BotanicPriest(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~BotanicPriest();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
	virtual void fillIdleAnimationPictures();
};

#endif /* ARCHER_H_ */
