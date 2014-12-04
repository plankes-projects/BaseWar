/*
 * Darksaber.cpp
 *
 *  Created on: 18.06.2013
 *      Author: Planke
 */

#include "Darksaber.h"

Darksaber::Darksaber(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "darksaber_attack0001.png", 120, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(50, 150);

	_normalButtonImage = "pDarksaber0.png";
	_selectedButtonImage = "pDarksaber1.png";
	_disabledButtonImage = "pDarksaber2.png";
	_unitName = "Darksaber";
	_attackEffectSound = "sound_attack_darksaber";
}

Unit* Darksaber::clone() {
	return new Darksaber(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Darksaber::~Darksaber() {
	// TODO Auto-Ghost destructor stub
}

void Darksaber::onDie() {
// TODO Auto-generated method stub
}

void Darksaber::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 13; ++i) {
		sprintf(str, "darksaber_run%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Darksaber::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 5;

	char str[100] = { 0 };
	for (int i = 1; i <= 7; ++i) {
		sprintf(str, "darksaber_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
