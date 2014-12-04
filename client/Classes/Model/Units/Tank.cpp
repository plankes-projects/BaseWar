/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Tank.h"

Tank::Tank(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "tank_fires0001.png", 150, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pTank1.png";
	_selectedButtonImage = "pTank2.png";
	_disabledButtonImage = "pTank3.png";
	_unitName = "Tank";
	_attackEffectSound = "sound_attack_explosion";
}

Unit* Tank::clone() {
	return new Tank(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Tank::~Tank() {
}

void Tank::onDie() {
}
void Tank::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 9; ++i) {
		sprintf(str, "tank_moves%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Tank::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 8;

	char str[100] = { 0 };
	for (int i = 1; i <= 11; ++i) {
		sprintf(str, "tank_fires%04d.png", i);
		addAttackAnimationPicture(str);
	}

	//add some pictures for reloading
	for (int i = 1; i <= 10; ++i) {
		addAttackAnimationPicture("tank_fires0001.png");
	}
}
