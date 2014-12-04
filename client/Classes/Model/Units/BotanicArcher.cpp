/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "BotanicArcher.h"

BotanicArcher::BotanicArcher(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills,
		float mana, float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "botanic_archer_attack0001.png", 80, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(15, 70);

	_normalButtonImage = "pBotanicArcher0.png";
	_selectedButtonImage = "pBotanicArcher1.png";
	_disabledButtonImage = "pBotanicArcher2.png";
	_unitName = "Ranger";
	_attackEffectSound = "sound_attack_bow";
}

Unit* BotanicArcher::clone() {
	return new BotanicArcher(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

BotanicArcher::~BotanicArcher() {
	// TODO Auto-generated destructor stub
}

void BotanicArcher::onDie() {
// TODO Auto-generated method stub
}
void BotanicArcher::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.02;

	char str[100] = { 0 };
	for (int i = 1; i <= 17; ++i) {
		sprintf(str, "botanic_archer_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void BotanicArcher::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 13;

	char str[100] = { 0 };
	for (int i = 1; i <= 22; ++i) {
		sprintf(str, "botanic_archer_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
