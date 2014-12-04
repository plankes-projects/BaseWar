/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef BotanicFemmage_H_
#define BotanicFemmage_H_

#include "MoveAbleUnit.h"

class BotanicFemmage: public MoveAbleUnit {
public:
	BotanicFemmage(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~BotanicFemmage();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* BotanicFemmage_H_ */
