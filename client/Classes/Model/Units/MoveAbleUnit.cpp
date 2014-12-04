/*
 * MoveAbleUnit.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "MoveAbleUnit.h"

MoveAbleUnit::MoveAbleUnit(int addCost, std::string image, float height, float runspeed, float hitpoints, Attack* attack, Armour* armour,
		std::list<Skill*> skills, float mana, float manaRegPerSeconds) :
		Unit(image, height, hitpoints, attack, armour, skills, mana, manaRegPerSeconds) {
	this->_runspeed = runspeed;
	_addCost = addCost;
	_roundsToLive = 3.5;
}

bool MoveAbleUnit::isMoveAble() {
	return true;
}
MoveAbleUnit::~MoveAbleUnit() {
	// TODO Auto-generated destructor stub
}

void MoveAbleUnit::moveTo(BW_Point location) {
	this->spriteMoveTo(location, getRunspeed(), false);
}

void MoveAbleUnit::fillIdleAnimationPictures() {
	//nothing to do
}
