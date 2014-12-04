/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "FlareTroll.h"

FlareTroll::FlareTroll(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "flare_troll_run0001.png", 80, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pFlareTroll1.png";
	_selectedButtonImage = "pFlareTroll2.png";
	_disabledButtonImage = "pFlareTroll3.png";
	_unitName = "Goblin";
	_attackEffectSound = "sound_attack_flaretroll";
}

Unit* FlareTroll::clone() {
	return new FlareTroll(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

FlareTroll::~FlareTroll() {
}

void FlareTroll::onDie() {
}
void FlareTroll::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.035;

	char str[100] = { 0 };
	for (int i = 1; i <= 15; ++i) {
		sprintf(str, "flare_troll_run00%02d.png", i);
		addMoveAnimationPicture(str);
	}
}

void FlareTroll::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 0;

	char str[100] = { 0 };
	for (int i = 1; i <= 26; ++i) {
		sprintf(str, "flare_troll_attack00%02d.png", i);
		addAttackAnimationPicture(str);
	}
}
