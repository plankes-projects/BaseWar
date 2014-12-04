/*
 * Viking.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Viking.h"

Viking::Viking(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "vikingswingB0001.png", 100, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(40, 90);

	_normalButtonImage = "pViking0.png";
	_selectedButtonImage = "pViking1.png";
	_disabledButtonImage = "pViking2.png";
	_unitName = "Viking";
	_attackEffectSound = "sound_attack_hammer";
}

Unit* Viking::clone() {
	return new Viking(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Viking::~Viking() {
	// TODO Auto-Ghost destructor stub
}

void Viking::onDie() {
// TODO Auto-generated method stub
}

void Viking::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.025;

	char str[100] = { 0 };
	for (int i = 1; i <= 19; ++i) {
		sprintf(str, "vikingwalk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Viking::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 10;

	char str[100] = { 0 };
	for (int i = 1; i <= 18; ++i) {
		sprintf(str, "vikingswingB%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
