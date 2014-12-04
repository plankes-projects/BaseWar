/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef BotanicDaemon_H_
#define BotanicDaemon_H_

#include "MoveAbleUnit.h"

class BotanicDaemon: public MoveAbleUnit {
public:
	BotanicDaemon(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~BotanicDaemon();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* BotanicDaemon_H_ */
