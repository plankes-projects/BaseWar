/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "BotanicDaemon.h"

BotanicDaemon::BotanicDaemon(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills,
		float mana, float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "botanic_daemon_attack0001.png", 200, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 70);

	_normalButtonImage = "pBotanicDaemon0.png";
	_selectedButtonImage = "pBotanicDaemon1.png";
	_disabledButtonImage = "pBotanicDaemon2.png";
	_unitName = "Daemon";
	_attackEffectSound = "sound_attack_daemon";
}

Unit* BotanicDaemon::clone() {
	return new BotanicDaemon(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

BotanicDaemon::~BotanicDaemon() {
	// TODO Auto-generated destructor stub
}

void BotanicDaemon::onDie() {
// TODO Auto-generated method stub
}
void BotanicDaemon::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.05;

	char str[100] = { 0 };
	for (int i = 1; i <= 12; ++i) {
		sprintf(str, "botanic_daemon_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void BotanicDaemon::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 13;

	char str[100] = { 0 };
	for (int i = 1; i <= 26; ++i) {
		sprintf(str, "botanic_daemon_attack%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
