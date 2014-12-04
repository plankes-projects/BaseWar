/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Archer.h"

Archer::Archer(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "archer_attack01.png", 80, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pArcher0.png";
	_selectedButtonImage = "pArcher1.png";
	_disabledButtonImage = "pArcher2.png";
	_unitName = "Archer";
	_attackEffectSound = "sound_attack_bow";
}

Unit* Archer::clone() {
	return new Archer(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Archer::~Archer() {
	// TODO Auto-generated destructor stub
}

void Archer::onDie() {
// TODO Auto-generated method stub
}
void Archer::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 20; ++i) {
		sprintf(str, "archer_move%02d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Archer::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 11;

	char str[100] = { 0 };
	for (int i = 1; i <= 12; ++i) {
		sprintf(str, "archer_attack%02d.png", i);
		addAttackAnimationPicture(str);
	}
}
