/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef BotanicArcher_H_
#define BotanicArcher_H_

#include "MoveAbleUnit.h"

class BotanicArcher: public MoveAbleUnit {
public:
	BotanicArcher(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~BotanicArcher();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* BotanicArcher_H_ */
