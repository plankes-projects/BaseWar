/*
 * Pinkie.cpp
 *
 *  Created on: 24.07.2013
 *      Author: Planke
 */

#include "Pinkie.h"

Pinkie::Pinkie(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "pknight_run0001.png", 100, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(50, 150);

	_normalButtonImage = "pPinkie0.png";
	_selectedButtonImage = "pPinkie1.png";
	_disabledButtonImage = "pPinkie2.png";
	_unitName = "Pinkie";
	_attackEffectSound = "";
}

Unit* Pinkie::clone() {
	return new Pinkie(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Pinkie::~Pinkie() {
	// TODO Auto-Ghost destructor stub
}

void Pinkie::onDie() {
// TODO Auto-generated method stub
}

void Pinkie::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 24; ++i) {
		sprintf(str, "pknight_run%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Pinkie::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 3;

	char str[100] = { 0 };
	for (int i = 1; i <= 6; ++i) {
		sprintf(str, "pkinght_spat%04d.png", i);
		addAttackAnimationPicture(str);
	}
}

void Pinkie::fillIdleAnimationPictures() {
	_idleAnimationSpeed = 0.04;

	char str[100] = { 0 };
	for (int i = 1; i <= 14; ++i) {
		sprintf(str, "PKnight_stance%04d.png", i);
		addIdleAnimationPicture(str);
	}
}
