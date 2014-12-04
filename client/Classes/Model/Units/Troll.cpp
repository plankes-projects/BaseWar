/*
 * Troll.cpp
 *
 *  Created on: 29.05.2013
 *      Author: Planke
 */

#include "Troll.h"

Troll::Troll(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "troll_attack0001.png", 100, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(40, 90);

	_normalButtonImage = "pTroll0.png";
	_selectedButtonImage = "pTroll1.png";
	_disabledButtonImage = "pTroll2.png";
	_unitName = "Troll";
	_attackEffectSound = "sound_attack_sword";
}

Unit* Troll::clone() {
	return new Troll(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Troll::~Troll() {
	// TODO Auto-Ghost destructor stub
}

void Troll::onDie() {
// TODO Auto-generated method stub
}

void Troll::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.025;

	char str[100] = { 0 };
	for (int i = 1; i <= 17; ++i) {
		sprintf(str, "troll_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Troll::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 14;

	char str[100] = { 0 };
	for (int i = 1; i <= 22; ++i) {
		sprintf(str, "troll_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
