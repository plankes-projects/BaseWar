/*
 * Minion.cpp
 *
 *  Created on: 24.07.2013
 *      Author: Planke
 */

#include "Minion.h"

Minion::Minion(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "minion_attack0001.png", 100, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(50, 150);

	_normalButtonImage = "pMinion0.png";
	_selectedButtonImage = "pMinion1.png";
	_disabledButtonImage = "pMinion2.png";
	_unitName = "Minion";
	_attackEffectSound = "sound_attack_sword";
}

Unit* Minion::clone() {
	return new Minion(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Minion::~Minion() {
	// TODO Auto-Ghost destructor stub
}

void Minion::onDie() {
// TODO Auto-generated method stub
}

void Minion::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 23; ++i) {
		sprintf(str, "minion_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Minion::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 8;

	char str[100] = { 0 };
	for (int i = 1; i <= 21; ++i) {
		sprintf(str, "minion_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
