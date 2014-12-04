/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "RemoveDebuffSkill.h"
#include "../Units/Unit.h"
#include "../Model.h"
#include "../../Tools/Tools.h"
#include "../Effects/ImmuneEffect.h"

RemoveDebuffSkill::~RemoveDebuffSkill() {
}

std::string RemoveDebuffSkill::getSkillInfo() {
	return "Removes a debuff from a friend and adds immunity for " + Tools::toString((int) (_immuneDuration / 1000)) + " sec. (Range: "
			+ Tools::toString((int) _range) + "; ";
}

RemoveDebuffSkill::RemoveDebuffSkill(float manaCost, float rechargeTime, float range, float immuneDuration) :
		Skill(manaCost, rechargeTime) {
	_range = range;
	_immuneDuration = immuneDuration;
}

Skill* RemoveDebuffSkill::clone() {
	return new RemoveDebuffSkill(_manaCost, _rechargeTime, _range, _immuneDuration);
}

bool RemoveDebuffSkill::perform(Unit* owner) {

	std::list<Unit*> friendUnits = Model::getInstance()->getMyArmy(owner->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = friendUnits.begin(); it != friendUnits.end(); ++it) {

		if ((*it)->getHitpoints() > 0 && owner->distanceTo((*it)) <= _range) {
			int effectGUID = (*it)->removeADebuff();
			if (effectGUID != 0) {
				(*it)->applyEffect(new ImmuneEffect(_immuneDuration, effectGUID));
				return true;
			}
		}
	}

	return false;
}
