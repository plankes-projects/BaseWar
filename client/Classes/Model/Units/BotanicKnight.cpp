/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "BotanicKnight.h"

BotanicKnight::BotanicKnight(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills,
		float mana, float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "botanic_knight_walk0001.png", 80, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pBotanicKnight0.png";
	_selectedButtonImage = "pBotanicKnight1.png";
	_disabledButtonImage = "pBotanicKnight2.png";
	_unitName = "Knight";
	_attackEffectSound = "sound_attack_sword";
}

Unit* BotanicKnight::clone() {
	return new BotanicKnight(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

BotanicKnight::~BotanicKnight() {
	// TODO Auto-generated destructor stub
}

void BotanicKnight::onDie() {
// TODO Auto-generated method stub
}
void BotanicKnight::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.025;

	char str[100] = { 0 };
	for (int i = 1; i <= 17; ++i) {
		sprintf(str, "botanic_knight_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void BotanicKnight::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 12;

	char str[100] = { 0 };
	for (int i = 1; i <= 16; ++i) {
		sprintf(str, "botanic_knight_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
