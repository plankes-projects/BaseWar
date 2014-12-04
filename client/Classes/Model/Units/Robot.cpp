/*
 * Robot.cpp
 *
 *  Created on: 24.07.2013
 *      Author: Planke
 */

#include "Robot.h"

Robot::Robot(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "ck_punch0001.png", 150, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(50, 150);

	_normalButtonImage = "pRobot0.png";
	_selectedButtonImage = "pRobot1.png";
	_disabledButtonImage = "pRobot2.png";
	_unitName = "Robot";
	_attackEffectSound = "sound_attack_robot";
}

Unit* Robot::clone() {
	return new Robot(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Robot::~Robot() {
	// TODO Auto-Ghost destructor stub
}

void Robot::onDie() {
// TODO Auto-generated method stub
}

void Robot::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.025;

	char str[100] = { 0 };
	for (int i = 1; i <= 17; ++i) {
		sprintf(str, "ck_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Robot::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 9;

	char str[100] = { 0 };
	for (int i = 1; i <= 17; ++i) {
		sprintf(str, "ck_punch%04d.png", i);
		addAttackAnimationPicture(str);
	}
}
