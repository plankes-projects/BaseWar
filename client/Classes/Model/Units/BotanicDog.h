/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef BotanicDog_H_
#define BotanicDog_H_

#include "MoveAbleUnit.h"

class BotanicDog: public MoveAbleUnit {
public:
	BotanicDog(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~BotanicDog();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* BotanicDog_H_ */
