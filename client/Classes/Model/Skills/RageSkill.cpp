/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "RageSkill.h"
#include "../Units/Unit.h"
#include "../Model.h"
#include "../Effects/RageEffect.h"
#include "../../Tools/Tools.h"

RageSkill::~RageSkill() {
}

std::string RageSkill::getSkillInfo() {
	return "Rage! (Range: " + Tools::toString((int) _range) + "; ";
}

RageSkill::RageSkill(float manaCost, float rechargeTime, float rangefloat, float effectTime, float increasedDamageInPercent,
		float increasedAttackSpeedInPercent, float increasedRunSpeedInPercent) :
		Skill(manaCost, rechargeTime) {
	_range = rangefloat;
	_increasedDamageInPercent = increasedDamageInPercent;
	_increasedAttackSpeedInPercent = increasedAttackSpeedInPercent;
	_increasedRunSpeedInPercent = increasedRunSpeedInPercent;
	_effectTime = effectTime;
}

Skill* RageSkill::clone() {
	return new RageSkill(_manaCost, _rechargeTime, _range, _effectTime, _increasedDamageInPercent, _increasedAttackSpeedInPercent,
			_increasedRunSpeedInPercent);
}

bool RageSkill::perform(Unit* owner) {

	//search enemy unit in range

	bool inRange = false;
	std::list<Unit*> enemyUnits = Model::getInstance()->getEnemyArmy(owner->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {

		if ((*it)->getHitpoints() > 0 && owner->distanceTo((*it)) <= _range) {
			inRange = true;
			break;
		}
	}

	if (!inRange)
		return false; //no action

	owner->applyEffect(new RageEffect(_effectTime, _increasedDamageInPercent, _increasedAttackSpeedInPercent, _increasedRunSpeedInPercent));

	return true;
}
