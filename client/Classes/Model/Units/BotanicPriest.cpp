/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "BotanicPriest.h"

BotanicPriest::BotanicPriest(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills,
		float mana, float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "botanic_priest_attack0001.png", 90, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pBotanicPriest0.png";
	_selectedButtonImage = "pBotanicPriest1.png";
	_disabledButtonImage = "pBotanicPriest2.png";
	_unitName = "Priest";
	_attackEffectSound = "";
}

Unit* BotanicPriest::clone() {
	return new BotanicPriest(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana,
			_manaRegPerSeconds);
}

BotanicPriest::~BotanicPriest() {
	// TODO Auto-generated destructor stub
}

void BotanicPriest::onDie() {
// TODO Auto-generated method stub
}
void BotanicPriest::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.02;

	char str[100] = { 0 };
	for (int i = 1; i <= 25; ++i) {
		sprintf(str, "botanic_priest_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void BotanicPriest::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 17;

	char str[100] = { 0 };
	for (int i = 1; i <= 22; ++i) {
		sprintf(str, "botanic_priest_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}

void BotanicPriest::fillIdleAnimationPictures() {
	_idleAnimationSpeed = 0.04;

	char str[100] = { 0 };
	for (int i = 1; i <= 11; ++i) {
		sprintf(str, "botanic_priest_idle%03d.png", i);
		addIdleAnimationPicture(str);
	}
}
