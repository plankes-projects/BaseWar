/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Slime.h"

Slime::Slime(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "slime_attack0019.png", 40, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pSlime0.png";
	_selectedButtonImage = "pSlime1.png";
	_disabledButtonImage = "pSlime2.png";
	_unitName = "Slime";
	_attackEffectSound = "sound_attack_slime";
}

Unit* Slime::clone() {
	return new Slime(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Slime::~Slime() {
}

void Slime::onDie() {
}
void Slime::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 14; ++i) {
		sprintf(str, "slime_run00%02d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Slime::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 13;

	char str[100] = { 0 };
	for (int i = 1; i <= 19; ++i) {
		sprintf(str, "slime_attack00%02d.png", i);
		addAttackAnimationPicture(str);
	}
}
