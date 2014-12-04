/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "TeleportAttack.h"
#include "../Units/Unit.h"
#include "../Model.h"
#include "../../Tools/Tools.h"
#include "../../View/Animations/SmokeBomb.h"

TeleportAttack::~TeleportAttack() {
}

std::string TeleportAttack::getSkillInfo() {

	return "Teleports to an enemy and does " + Tools::toString(_damageMultiplier, 2) + "x damage. (Range: " + Tools::toString((int) _range) + "; ";
}

TeleportAttack::TeleportAttack(float manaCost, float rechargeTime, float range, float damageMultiplier) :
		Skill(manaCost, rechargeTime) {
	_range = range;
	_damageMultiplier = damageMultiplier;
}

Skill* TeleportAttack::clone() {
	return new TeleportAttack(_manaCost, _rechargeTime, _range, _damageMultiplier);
}

bool TeleportAttack::perform(Unit* owner) {

	//search unit in range with lowest health
	Unit* lowestHealthTarget = NULL;
	float lowestHealth = 0;

	std::list<Unit*> enemyUnits = Model::getInstance()->getEnemyArmy(owner->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {

		if ((*it)->getHitpoints() > 0 && (lowestHealth == 0 || (*it)->getHitpoints() < lowestHealth)
				&& owner->distanceTo((*it)) <= _range) {
			lowestHealthTarget = (*it);
			lowestHealth = lowestHealthTarget->getHitpoints();
		}
	}

	if (lowestHealthTarget == NULL)
		return false; //no action

	float smokeBombHeight = owner->getContentSize().getHeight();
	float smokeBombYDiff = (owner->getContentSize().getHeight() - smokeBombHeight) / 2;
	BW_Point smokeBombPos = owner->getPositionBW();
	smokeBombPos.y -= smokeBombYDiff;
	(new SmokeBomb())->animateAt(owner->getParent(), smokeBombPos, smokeBombHeight, false);

	//teleport unit behind target
	BW_Point newPos = lowestHealthTarget->getPositionBW();
	Attack* attack = owner->getAttack();
	float intConvert = 10000;
	float atackRange = attack->getAttackRange();
	float y = atackRange * intConvert;
	y = Tools::random(-y, y) / intConvert;

	newPos.y += y;

	//calculate a^2+b^2 = c^2
	float x = (atackRange * atackRange) - (y * y);
	if (x < 0)
		x *= -1;

	if (lowestHealthTarget->getLookingTo() == LEFT)
		newPos.x += sqrt(x);
	else
		newPos.x -= sqrt(x);

	owner->setPosition(newPos);
	owner->setLookingTo(lowestHealthTarget->getPositionBW(), false);

	lowestHealthTarget->Receivedamage(attack->getDamagePerSecond() * _damageMultiplier, attack->getDamageType());

	smokeBombPos = newPos;
	smokeBombPos.y -= smokeBombYDiff;
	(new SmokeBomb())->animateAt(owner->getParent(), smokeBombPos, smokeBombHeight, false);

	return true;
}
