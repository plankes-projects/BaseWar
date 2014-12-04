/*
 * Soldier.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Soldier.h"
#include "../Attacks/Attack.h"

Soldier::Soldier(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "soldier_shootA0001.png", 80, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pSoldier0.png";
	_selectedButtonImage = "pSoldier1.png";
	_disabledButtonImage = "pSoldier2.png";
	_unitName = "Soldier";
	_attackEffectSound = "sound_attack_explosion";
}

Unit* Soldier::clone() {
	return new Soldier(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Soldier::~Soldier() {
	// TODO Auto-generated destructor stub
}

void Soldier::onDie() {
// TODO Auto-generated method stub
}
void Soldier::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 18; ++i) {
		sprintf(str, "soldier_run00%02d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Soldier::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 5;

	char str[100] = { 0 };
	for (int i = 1; i <= 25; ++i) {
		sprintf(str, "soldier_shootA00%02d.png", i);
		addAttackAnimationPicture(str);
	}

	//add some pictures for reloading
	for (int i = 1; i <= 10; ++i) {
		addAttackAnimationPicture("soldier_shootA0001.png");
	}
}
