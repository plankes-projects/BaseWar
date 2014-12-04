/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Golem.h"

Golem::Golem(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "archer_attack01.png", 200, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 180);

	_normalButtonImage = "pGolem0.png";
	_selectedButtonImage = "pGolem1.png";
	_disabledButtonImage = "pGolem2.png";
	_unitName = "Golem";
	_attackEffectSound = "sound_attack_golem";
}

Unit* Golem::clone() {
	return new Golem(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Golem::~Golem() {
}

void Golem::onDie() {
}

void Golem::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 21; ++i) {
		sprintf(str, "golem_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Golem::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 11;

	char str[100] = { 0 };
	for (int i = 1; i <= 17; ++i) {
		sprintf(str, "golem_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
