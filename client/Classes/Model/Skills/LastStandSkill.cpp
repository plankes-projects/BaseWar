/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "LastStandSkill.h"
#include "../Units/Unit.h"
#include "../../Tools/Tools.h"
#include "../Effects/DefenceEffect.h"

LastStandSkill::~LastStandSkill() {
}

std::string LastStandSkill::getSkillInfo() {
	return "Last Stand (At: " + Tools::toString((int) (_onPercentLife * 100)) + "% HP; ";
}

LastStandSkill::LastStandSkill(float manaCost, float rechargeTime, float onPercentLife, float duration, float increaseLifeByPercent,
		float increaseArmourByPercent) :
		Skill(manaCost, rechargeTime) {
	_onPercentLife = onPercentLife;
	_duration = duration;
	_increaseLifeByPercent = increaseLifeByPercent;
	_increaseArmourByPercent = increaseArmourByPercent;
}

Skill* LastStandSkill::clone() {
	return new LastStandSkill(_manaCost, _rechargeTime, _onPercentLife, _duration, _increaseLifeByPercent, _increaseArmourByPercent);
}

bool LastStandSkill::perform(Unit* owner) {

	//launching at x% and if we are in combat
	if (owner->getHitpoints() <= 0 || owner->getHitpoints() / owner->getMaxHitpoints() > _onPercentLife || !owner->isAttacked())
		return false;

	//apply effect
	owner->applyEffect(new DefenceEffect(_duration, _increaseLifeByPercent, _increaseArmourByPercent));

	return true;
}
