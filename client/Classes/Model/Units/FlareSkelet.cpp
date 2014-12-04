/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "FlareSkelet.h"

FlareSkelet::FlareSkelet(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "flare_skelet_attack0001.png", 80, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pFlareSkelet1.png";
	_selectedButtonImage = "pFlareSkelet2.png";
	_disabledButtonImage = "pFlareSkelet3.png";
	_unitName = "Skeleton";
	_attackEffectSound = "sound_attack_sword";
}

Unit* FlareSkelet::clone() {
	return new FlareSkelet(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

FlareSkelet::~FlareSkelet() {
}

void FlareSkelet::onDie() {
}
void FlareSkelet::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.045;

	char str[100] = { 0 };
	for (int i = 1; i <= 14; ++i) {
		sprintf(str, "flare_skelet_run00%02d.png", i);
		addMoveAnimationPicture(str);
	}
}

void FlareSkelet::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 5;

	char str[100] = { 0 };
	for (int i = 1; i <= 12; ++i) {
		sprintf(str, "flare_skelet_attack00%02d.png", i);
		addAttackAnimationPicture(str);
	}
}
