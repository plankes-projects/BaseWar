/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef BotanicMage_H_
#define BotanicMage_H_

#include "MoveAbleUnit.h"

class BotanicMage: public MoveAbleUnit {
public:
	BotanicMage(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~BotanicMage();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* ARCHER_H_ */
