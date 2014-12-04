/*
 * InstantSplashAttack.cpp
 *
 *  Created on: 30.05.2013
 *      Author: Planke
 */

#include "ManaBurnSkill.h"
#include "../Units/Unit.h"
#include "../Model.h"
#include "../../Tools/Tools.h"

ManaBurnSkill::~ManaBurnSkill() {
}

std::string ManaBurnSkill::getSkillInfo() {
	return "Burns " + Tools::toString((int) _manaCost * _manaBurnPerManaUsed) + " mana of an enemy. (Range: "
			+ Tools::toString((int) _range) + "; ";
}

ManaBurnSkill::ManaBurnSkill(float manaCost, float rechargeTime, float range, float manaBurnPerManaUsed) :
		Skill(manaCost, rechargeTime) {
	_range = range;
	_manaBurnPerManaUsed = manaBurnPerManaUsed;
}

Skill* ManaBurnSkill::clone() {
	return new ManaBurnSkill(_manaCost, _rechargeTime, _range, _manaBurnPerManaUsed);
}

bool ManaBurnSkill::perform(Unit* owner) {

	//search enemy with max mana in range
	Unit* unitWithMaxMana = NULL;
	std::list<Unit*> enemyUnits = Model::getInstance()->getEnemyArmy(owner->getArmyType())->getUnits();
	for (std::list<Unit*>::iterator it = enemyUnits.begin(); it != enemyUnits.end(); ++it) {

		if ((*it)->getHitpoints() > 0 && owner->distanceTo((*it)) <= _range) {
			if (unitWithMaxMana == NULL || unitWithMaxMana->getMana() < (*it)->getMana())
				unitWithMaxMana = (*it);
		}
	}

	if(unitWithMaxMana == NULL)
		return false;

	//burn mana now
	float burn = _manaCost * _manaBurnPerManaUsed;
	unitWithMaxMana->reduceManaBy(burn);
	unitWithMaxMana->Receivedamage(burn, MAGIC);

	return true;
}
