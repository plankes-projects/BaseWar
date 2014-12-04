/*
 * Archer.h
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#ifndef FlareAntlion_H_
#define FlareAntlion_H_

#include "MoveAbleUnit.h"

class FlareAntlion: public MoveAbleUnit {
public:
	FlareAntlion(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
			float manaRegPerSeconds);
	virtual ~FlareAntlion();
	void onDie();
	virtual Unit* clone();
private:
	virtual void fillMoveAnimationPictures();
	virtual void fillAttackAnimationPictures();
};

#endif /* ARCHER_H_ */
