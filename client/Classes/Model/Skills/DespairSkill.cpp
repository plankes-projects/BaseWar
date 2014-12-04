/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "DespairSkill.h"
#include "../Units/Unit.h"
#include "../Model.h"
#include "../Effects/RageEffect.h"
#include "../../Tools/Tools.h"

DespairSkill::~DespairSkill() {
}

std::string DespairSkill::getSkillInfo() {
	return "Despair! (At: " + Tools::toString((int) (_onPercentLife * 100)) + "% HP; ";
}

DespairSkill::DespairSkill(float manaCost, float rechargeTime, float effectTime, float onPercentLife,
		float increasedDamageInPercent, float increasedAttackSpeedInPercent) :
		Skill(manaCost, rechargeTime) {
	_increasedDamageInPercent = increasedDamageInPercent;
	_increasedAttackSpeedInPercent = increasedAttackSpeedInPercent;
	_effectTime = effectTime;
	_onPercentLife = onPercentLife;
}

Skill* DespairSkill::clone() {
	return new DespairSkill(_manaCost, _rechargeTime, _effectTime, _onPercentLife, _increasedDamageInPercent,
			_increasedAttackSpeedInPercent);
}

bool DespairSkill::perform(Unit* owner) {

	if (owner->getHitpoints() <= 0 || owner->getHitpoints() / owner->getMaxHitpoints() > _onPercentLife || !owner->isAttacked())
			return false;


	owner->applyEffect(new RageEffect(_effectTime, _increasedDamageInPercent, _increasedAttackSpeedInPercent, 0));

	return true;
}
