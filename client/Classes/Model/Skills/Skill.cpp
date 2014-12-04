/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "Skill.h"
#include "../../Tools/BW_Time.h"
#include "../Units/Unit.h"
#include "../../Tools/Tools.h"
#include "../Model.h"

Skill::~Skill() {
}

Skill::Skill(float manaCost, float rechargeTime) {
	_upgradeNum = 0;
	_upgradeMultiplier = 1.15; //standard value
	_rechargeTime = rechargeTime < 1000 ? 1000 : rechargeTime; //1 seconds mindest recharge time
	_lastperform = 0;
	_lastCheckedHP = 0;
	_manaCost = manaCost;
}

std::string Skill::getInfo() {
	return getSkillInfo() + "Mana: " + Tools::toString(_manaCost) + ")";
}

void Skill::tryPerform(Unit* owner) {
	if (owner->getMana() < _manaCost)
		return; //not enough mana

	if (_lastperform >= BW_Time::getMilliSecondsCached() - _rechargeTime)
		return; //spell not ready yet

	float lastCheckedHpTmp = _lastCheckedHP;
	_lastCheckedHP = owner->getHitpoints();
	_ownerIsAttacked = lastCheckedHpTmp != _lastCheckedHP;

	if (perform(owner)) {
		_lastperform = BW_Time::getMilliSecondsCached();
		owner->reduceManaBy(_manaCost);
	}
}
