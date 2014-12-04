/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "BotanicDog.h"

BotanicDog::BotanicDog(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "botanic_dog_attack0001.png", 40, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations

	_normalButtonImage = "pBotanicDog0.png";
	_selectedButtonImage = "pBotanicDog1.png";
	_disabledButtonImage = "pBotanicDog2.png";
	_unitName = "Dog";
	_attackEffectSound = "sound_attack_dog";
}

Unit* BotanicDog::clone() {
	return new BotanicDog(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

BotanicDog::~BotanicDog() {
	// TODO Auto-generated destructor stub
}

void BotanicDog::onDie() {
// TODO Auto-generated method stub
}
void BotanicDog::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.025;

	char str[100] = { 0 };
	for (int i = 1; i <= 16; ++i) {
		sprintf(str, "botanic_dog_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void BotanicDog::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 18;

	char str[100] = { 0 };
	for (int i = 1; i <= 20; ++i) {
		sprintf(str, "botanic_dog_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
