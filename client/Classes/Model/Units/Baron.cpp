/*
 * Baron.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Baron.h"

Baron::Baron(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "baron_attackA0010.png", 150, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(50, 150);

	_normalButtonImage = "pBaron0.png";
	_selectedButtonImage = "pBaron1.png";
	_disabledButtonImage = "pBaron2.png";
	_unitName = "Baron";
	_attackEffectSound = "sound_attack_baron";
}

Unit* Baron::clone() {
	return new Baron(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Baron::~Baron() {
	// TODO Auto-Ghost destructor stub
}

void Baron::onDie() {
// TODO Auto-generated method stub
}

void Baron::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 15; ++i) {
		sprintf(str, "baron_run%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Baron::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 6;

	char str[100] = { 0 };
	for (int i = 1; i <= 11; ++i) {
		sprintf(str, "baron_attackA%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
