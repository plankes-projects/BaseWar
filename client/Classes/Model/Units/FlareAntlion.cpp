/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "FlareAntlion.h"

FlareAntlion::FlareAntlion(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills,
		float mana, float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "flare_antlion_attack0001.png", 80, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pFlareAntlion1.png";
	_selectedButtonImage = "pFlareAntlion2.png";
	_disabledButtonImage = "pFlareAntlion3.png";
	_unitName = "Antlion";
	_attackEffectSound = "sound_attack_antlion";
}

Unit* FlareAntlion::clone() {
	return new FlareAntlion(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

FlareAntlion::~FlareAntlion() {
}

void FlareAntlion::onDie() {
}
void FlareAntlion::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.03;

	char str[100] = { 0 };
	for (int i = 1; i <= 15; ++i) {
		sprintf(str, "flare_antlion_run00%02d.png", i);
		addMoveAnimationPicture(str);
	}
}

void FlareAntlion::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 8;

	char str[100] = { 0 };
	for (int i = 1; i <= 12; ++i) {
		sprintf(str, "flare_antlion_attack00%02d.png", i);
		addAttackAnimationPicture(str);
	}
}
