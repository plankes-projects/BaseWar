/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "BotanicFemmage.h"

BotanicFemmage::BotanicFemmage(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills,
		float mana, float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "botanic_femmage_attack0001.png", 100, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(15, 70);

	_normalButtonImage = "pBotanicFemmage0.png";
	_selectedButtonImage = "pBotanicFemmage1.png";
	_disabledButtonImage = "pBotanicFemmage2.png";
	_unitName = "Frostmage";
	_attackEffectSound = "sound_attack_spell";
}

Unit* BotanicFemmage::clone() {
	return new BotanicFemmage(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

BotanicFemmage::~BotanicFemmage() {
	// TODO Auto-generated destructor stub
}

void BotanicFemmage::onDie() {
// TODO Auto-generated method stub
}
void BotanicFemmage::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.025;

	char str[100] = { 0 };
	for (int i = 1; i <= 13; ++i) {
		sprintf(str, "botanic_femmage_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void BotanicFemmage::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 26;

	char str[100] = { 0 };
	for (int i = 1; i <= 33; ++i) {
		sprintf(str, "botanic_femmage_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
