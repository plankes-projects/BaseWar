/*
 * NinjaMelee.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#include "NinjaMelee.h"

NinjaMelee::NinjaMelee(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "ninja_stood.png", 100, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	//_hitBox = Rect(40, 90);

	_normalButtonImage = "pNinjaMelee0.png";
	_selectedButtonImage = "pNinjaMelee1.png";
	_disabledButtonImage = "pNinjaMelee2.png";
	_unitName = "MeleeNinja";
	_attackEffectSound = "sound_attack_sword";
}

Unit* NinjaMelee::clone() {
	return new NinjaMelee(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana,
			_manaRegPerSeconds);
}

NinjaMelee::~NinjaMelee() {
	// TODO Auto-Ghost destructor stub
}

void NinjaMelee::onDie() {
// TODO Auto-generated method stub
}

void NinjaMelee::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.1;

	char str[100] = { 0 };
	for (int i = 1; i <= 6; ++i) {
		sprintf(str, "ninja_walking%d.png", i);
		addMoveAnimationPicture(str);
	}
}

void NinjaMelee::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 8;

	char str[100] = { 0 };
	for (int i = 1; i <= 8; ++i) {
		sprintf(str, "ninja_striking%d.png", i);
		addAttackAnimationPicture(str);
	}
}
