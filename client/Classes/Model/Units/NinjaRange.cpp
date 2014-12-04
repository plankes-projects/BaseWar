/*
 * NinjaRange.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Planke
 */

#include "NinjaRange.h"

NinjaRange::NinjaRange(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "ninja_stood.png", 100, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	//_hitBox = Rect(40, 90);
	_normalButtonImage = "pNinjaRange0.png";
	_selectedButtonImage = "pNinjaRange1.png";
	_disabledButtonImage = "pNinjaRange2.png";
	_unitName = "RangeNinja";
	_attackEffectSound = "sound_attack_spell";
}

Unit* NinjaRange::clone() {
	return new NinjaRange(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

NinjaRange::~NinjaRange() {
	// TODO Auto-Ghost destructor stub
}

void NinjaRange::onDie() {
// TODO Auto-generated method stub
}

void NinjaRange::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.1;

	char str[100] = { 0 };
	for (int i = 1; i <= 6; ++i) {
		sprintf(str, "ninja_walking%d.png", i);
		addMoveAnimationPicture(str);
	}
}

void NinjaRange::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 7;

	char str[100] = { 0 };
	for (int i = 1; i <= 7; ++i) {
		sprintf(str, "ninja_throwing%d.png", i);
		addAttackAnimationPicture(str);
	}
}
