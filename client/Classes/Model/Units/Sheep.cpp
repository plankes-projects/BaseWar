/*
 * Archer.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "Sheep.h"
#include "../Attacks/Attack.h"

Sheep::Sheep(int addCost, float runSpeed, float hitPoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana,
		float manaRegPerSeconds) :
		MoveAbleUnit(addCost, "sheep_walk0001.png", 40, runSpeed, hitPoints, attack, armour, skills, mana, manaRegPerSeconds) {
	initAnimations(); //loading the pictures and set the animations

	_normalButtonImage = "pSheep0.png";
	_selectedButtonImage = "pSheep1.png";
	_disabledButtonImage = "pSheep2.png";
	_unitName = "Sheep";
	_attackEffectSound = "sound_attack_sheep";
}

Unit* Sheep::clone() {
	return new Sheep(_addCost, _runspeed, _maxHitpoints, _attack->clone(), _armour->clone(), cloneSkills(_skills), _mana, _manaRegPerSeconds);
}

Sheep::~Sheep() {
	// TODO Auto-generated destructor stub
}

void Sheep::onDie() {
// TODO Auto-generated method stub
}
void Sheep::fillMoveAnimationPictures() {
	_moveAnimationSpeed = 0.02;

	char str[100] = { 0 };
	for (int i = 1; i <= 20; ++i) {
		sprintf(str, "sheep_walk%04d.png", i);
		addMoveAnimationPicture(str);
	}
}

void Sheep::fillAttackAnimationPictures() {
	_animationDoDmgPicture = 0;
	//no attack animation
}
