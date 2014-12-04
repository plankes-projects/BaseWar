/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "ReduceReceivedHealSkill.h"
#include "../Units/Unit.h"
#include "../Model.h"
#include "../Effects/AlterHealReceivedEffect.h"
#include "../../Tools/Tools.h"

ReduceReceivedHealSkill::~ReduceReceivedHealSkill() {
}

std::string ReduceReceivedHealSkill::getSkillInfo() {
	return "Reduces received heal by " + Tools::toString((int) (_percent * 100)) + "%. (Range: " + Tools::toString((int) _range) + "; ";
}

ReduceReceivedHealSkill::ReduceReceivedHealSkill(float manaCost, float rechargeTime, float range, float effectTime, float percent) :
		Skill(manaCost, rechargeTime) {
	_percent = percent;
	_range = range;
	_effectTime = effectTime;
}

Skill* ReduceReceivedHealSkill::clone() {
	return new ReduceReceivedHealSkill(_manaCost, _rechargeTime, _range, _effectTime, _percent);
}

bool ReduceReceivedHealSkill::perform(Unit* owner) {

	Unit* target = NULL;

	Effect* effect = new AlterHealReceivedEffect(_effectTime, _percent, true);

	std::list<Unit*> enemyUnits = Model::getInstance()->getEnemyArmy(owner->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {
		if ((*it)->getHitpoints() > 0 && (*it)->isAttacked() && owner->distanceTo((*it)) <= _range) {
			//now check if he already have this debuff applied
			if (!(*it)->isImmuneToEffect(effect) && !(*it)->hasEffect(effect))
				target = (*it);
		}
	}

	if (target == NULL)
	{
		delete effect;
		return false; //no action
	}

	target->applyEffect(effect);
	return true;
}
