/*
 * ModernSoldier.cpp
 *
 *  Created on: 02.06.2013
 *      Author: Planke
 */

#include "ModernSoldier.h"

ModernSoldier::ModernSoldier(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills,
		float mana, float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "standing_modernSoldier.png", 100, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	//_hitBox = Rect(40, 90);

	_normalButtonImage = "pModernSoldier0.png";
	_selectedButtonImage = "pModernSoldier1.png";
	_disabledButtonImage = "pModernSoldier2.png";
	_unitName = "ModernSoldier";
	_attackEffectSound = "sound_attack_gun";
}

Unit* ModernSoldier::clone() {
	return new ModernSoldier(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

ModernSoldier::~ModernSoldier() {
	// TODO Auto-Ghost destructor stub
}

void ModernSoldier::onDie() {
// TODO Auto-generated method stub
}

void ModernSoldier::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.075;

	char str[100] = { 0 };
	for (int i = 1; i <= 7; ++i) {
		sprintf(str, "running%d_modernSoldier.png", i);
		addMoveAnimationPicture(str);
	}
}

void ModernSoldier::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 0;

	char str[100] = { 0 };
	for (int i = 1; i <= 7; ++i) {
		sprintf(str, "shooting%d_modernSoldier.png", i);
		addAttackAnimationPicture(str);
	}
}
