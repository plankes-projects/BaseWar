/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "RemoveBuffSkill.h"
#include "../Units/Unit.h"
#include "../Model.h"
#include "../../Tools/Tools.h"

RemoveBuffSkill::~RemoveBuffSkill() {
}

std::string RemoveBuffSkill::getSkillInfo() {
	return "Removes a buff from an enemy. (Range: " + Tools::toString((int) _range) + "; ";
}

RemoveBuffSkill::RemoveBuffSkill(float manaCost, float rechargeTime, float range) :
		Skill(manaCost, rechargeTime) {
	_range = range;
}

Skill* RemoveBuffSkill::clone() {
	return new RemoveBuffSkill(_manaCost, _rechargeTime, _range);
}

bool RemoveBuffSkill::perform(Unit* owner) {

	std::list<Unit*> enemyUnits = Model::getInstance()->getEnemyArmy(owner->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {

		if ((*it)->getHitpoints() > 0 && owner->distanceTo((*it)) <= _range && (*it)->removeABuff()) {
			return true;
		}
	}

	return false;
}
