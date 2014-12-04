/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Defector.h"

Defector::Defector(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "defect_throw0001.png", 100, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pDefector0.png";
	_selectedButtonImage = "pDefector1.png";
	_disabledButtonImage = "pDefector2.png";
	_unitName = "Defector";
	_attackEffectSound = "sound_attack_sword";
}

Unit* Defector::clone() {
	return new Defector(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Defector::~Defector() {
}

void Defector::onDie() {
}
void Defector::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.02;

	char str[100] = { 0 };
	for (int i = 1; i <= 24; ++i) {
		sprintf(str, "defect_walk00%02d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Defector::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 0;

	char str[100] = { 0 };
	for (int i = 1; i <= 8; ++i) {
		sprintf(str, "defect_throw00%02d.png", i);
		addAttackAnimationPicture(str);
	}
}
