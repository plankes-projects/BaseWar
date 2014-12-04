/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "BotanicMage.h"

BotanicMage::BotanicMage(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "botanic_mage_attack00001.png", 80, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(15, 70);

	_normalButtonImage = "pBotanicMage0.png";
	_selectedButtonImage = "pBotanicMage1.png";
	_disabledButtonImage = "pBotanicMage2.png";
	_unitName = "Mage";
	_attackEffectSound = "sound_attack_spell";
}

Unit* BotanicMage::clone() {
	return new BotanicMage(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

BotanicMage::~BotanicMage() {
	// TODO Auto-generated destructor stub
}

void BotanicMage::onDie() {
// TODO Auto-generated method stub
}
void BotanicMage::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.02;

	char str[100] = { 0 };
	for (int i = 1; i <= 25; ++i) {
		sprintf(str, "botanic_mage_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void BotanicMage::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 16;

	char str[100] = { 0 };
	for (int i = 1; i <= 18; ++i) {
		sprintf(str, "botanic_mage_attack%05d.png", i);
		addAttackAnimationPicture(str);
	}
}
