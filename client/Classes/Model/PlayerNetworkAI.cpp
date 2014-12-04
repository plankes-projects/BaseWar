/*
 * GameStat.cpp
 *
 *  Created on: 20.07.2013
 *      Author: Planke
 */

#include "PlayerNetworkAI.h"
#include "Player.h"

#include "Model.h"
#include "Units/UnitFactory.h"
#include "../Tools/Tools.h"
#include <map>

PlayerNetworkAI::PlayerNetworkAI(float strength) {
	_strength = strength;
	//we do not want to have a lower AI than 0.5
	if (_strength < 0.5)
		_strength = 0.5;
	if (_strength > 1)
		_strength = 1;
}

PlayerNetworkAI::~PlayerNetworkAI() {

}

void PlayerNetworkAI::performFor(Player* player) {
	if (Tools::random(0, 100) <= _strength * 100) {
		//mighty new build strategy
		if (!upgradeTier(player)) {
			return;
		}

		Player* enemy;
		if (player->getArmyTyp() == LEFT) {
			//left one is building versus first
			enemy = Model::getInstance()->getEnemyArmy(player->getArmyTyp())->getPlayers().front();
		} else {
			//right one is building vs last (since left is updated first and gets pushed to the end of the list)
			enemy = Model::getInstance()->getEnemyArmy(player->getArmyTyp())->getPlayers().back();
		}

		CCLOG("---- %s is building vs %s ----", player->getName().c_str(), enemy->getName().c_str());

		//if enemy has higher tier as me, we upgrade to 25% chance
		if (player->getCurrentTier() < enemy->getCurrentTier() && Tools::random(0, 3) == 0) {
			if (!upgradeTier(player, true)) {
				//update did not work, save money for next time
				return;
			}
		}
		upgradeGold(player);
		buildUnitDispatcher(player, enemy);
	} else {
		//stupid old build strategy
		player->performAIActions();
	}
}

bool PlayerNetworkAI::upgradeTier(Player* player, bool force) {
	Tier currentTier = player->getCurrentTier();
	if (currentTier == Tier3)
		return true;

	bool upgrade = force;
	if (!upgrade) {
		float absoluteGold = player->getGoldAbsolute();

		int randNum = Tools::random(0, 400) + absoluteGold;

		if (currentTier == Tier1 && randNum > 2000 && Tools::random(0, 4) != 1) {
			upgrade = true;
		} else if (currentTier == Tier2 && randNum > 4000 && Tools::random(0, 4) != 1) {
			upgrade = true;
		}
	}

	if (upgrade) {
		if (player->getCurrentTier() == Tier1) {
			if (player->upgradeToTier2()) {
				CCLOG("Upgraded to TIER2");
				return true;
			}
			return false;
		} else if (player->getCurrentTier() == Tier2) {
			if (player->upgradeToTier3()) {
				CCLOG("Upgraded to TIER3");
				return true;
			}
			return false;
		} else {
			return true;
		}
	}

	return true;
}

void PlayerNetworkAI::upgradeGold(Player* player, bool force) {
	bool upgrade = force;
	if (!upgrade) {
		//we upgrade if we have middle and max with 200g
		if (player->getincomeUpgradeCost() <= 200 && Model::getInstance()->getMiddleControl() == player->getArmyTyp()) {
			upgrade = true;
		}
	}

	if (upgrade) {
		player->upgradeIncome();
	}
}

void PlayerNetworkAI::buildUnitDispatcher(Player* player, Player* enemy) {
	CCLOG("Dispatcher called");
	std::vector<UnitType> type;
	type.push_back(TANK);
	type.push_back(DPS);

	if (Tools::random(0, 4) == 1) {
		type.push_back(SUPPORT);
	}

	while (type.size() != 0) {
		int index = Tools::random(0, type.size() - 1);
		switch (type[index]) {
		case TANK:
			buildTank(player, enemy);
			return;
		case DPS:
			buildDPS(player, enemy);
			return;
		case SUPPORT:
			buildSupport(player, enemy);
			break;
		}
		type.erase(type.begin() + index);
	}

}

void PlayerNetworkAI::buildTank(Player* player, Player* enemy) {
	CCLOG("Building TANK");
	std::vector<UnitFactory*> tanks = getPossibleUnits(player, TANK);

	if (tanks.size() == 0) {
		//no tank available
		return;
	}

	std::vector<UnitFactory*> tanksToBuild;
	if (Tools::random(0, 100) <= _strength * 100) {
		//good build

		DamageType maxDamageType = NORMAL;
		float maxDPS = 0;

		// 1. select damage type with highest value
		std::map<DamageType, float> dpsMap = getDPSPerDamageType(enemy);
		for (std::map<DamageType, float>::iterator iterator = dpsMap.begin(); iterator != dpsMap.end(); iterator++) {
			if (iterator->second > maxDPS) {
				maxDamageType = iterator->first;
				maxDPS = iterator->second;
			}
		}

		// 2. build melee unit with this key as damage reduction
		for (std::vector<UnitFactory*>::iterator uf = tanks.begin(); uf != tanks.end(); ++uf) {
			UnitFactory* tank = (*uf);
			std::vector<DamageType> damageReduction = tank->getUnit()->getArmour()->getDamageReductionType();
			if (std::find(damageReduction.begin(), damageReduction.end(), maxDamageType) != damageReduction.end()) {
				/* v contains x */
				//good unit
				tanksToBuild.push_back(tank);
			}
		}
	}

	if (tanksToBuild.size() == 0) {
		//bad build
		CCLOG("Building bad tank");

		if (Tools::random(0, 1) == 1) {
			upgradeTier(player, true);
			return;	//do not build bad units with 50% chance
		}

		tanksToBuild = tanks;
	}

	//now build randomly tanks
	while (buildUnitOrUpgrade(tanksToBuild[Tools::random(0, tanksToBuild.size() - 1)]))
		;
}

void PlayerNetworkAI::buildDPS(Player* player, Player* enemy) {
	CCLOG("Building DPS");
	std::vector<UnitFactory*> allDps = getPossibleUnits(player, DPS);

	if (allDps.size() == 0) {
		//no dps available
		return;
	}

	std::vector<UnitFactory*> dpsToBuild;
	if (Tools::random(0, 100) <= _strength * 100) {
		//good build

		float lowestHealth = -1;

		// 1. select entry with lowest value
		std::map<DamageType, float> toughnessMap = getToughnessOfMeleeUnits(enemy);
		for (std::map<DamageType, float>::iterator iterator = toughnessMap.begin(); iterator != toughnessMap.end(); iterator++) {
			if (lowestHealth == -1 || iterator->second < lowestHealth) {
				lowestHealth = iterator->second;
			}
		}

		//add all good damage types of the same lowest value
		std::vector<DamageType> goodDamageType;
		for (std::map<DamageType, float>::iterator iterator = toughnessMap.begin(); iterator != toughnessMap.end(); iterator++) {
			if (lowestHealth == iterator->second) {
				goodDamageType.push_back(iterator->first);
			}
		}

		// 2. build unit with this key as damageType
		for (std::vector<UnitFactory*>::iterator uf = allDps.begin(); uf != allDps.end(); ++uf) {
			UnitFactory* dps = (*uf);
			if (std::find(goodDamageType.begin(), goodDamageType.end(), dps->getAttack()->getDamageType()) != goodDamageType.end()) {
				/* v contains x */
				//good unit
				dpsToBuild.push_back(dps);
			}
		}
	}

	if (dpsToBuild.size() == 0) {
		//bad build
		CCLOG("Building bad dps");
		if (Tools::random(0, 1) == 1) {
			upgradeTier(player, true);
			return;	//do not build bad units with 50% chance
		}
		dpsToBuild = allDps;
	}

	//now build randomly dps
	while (buildUnitOrUpgrade(dpsToBuild[Tools::random(0, dpsToBuild.size() - 1)]))
		;

}

void PlayerNetworkAI::buildSupport(Player* player, Player* enemy) {
	CCLOG("Building SUPPORT");
	//support unit is healer

	std::vector<UnitFactory*> healers = getPossibleUnits(player, SUPPORT);

	if (healers.size() == 0) {
		//no healers available
		return;
	}

	//now build randomly healers
	while (buildUnitOrUpgrade(healers[Tools::random(0, healers.size() - 1)]))
		;
}

bool PlayerNetworkAI::buildUnitOrUpgrade(UnitFactory* factory) {
	//MELEE_UNIT_RECOGNITION_RANGE
	// we have decide, upgrade or build?

	if (Tools::random(0, 100) <= _strength * 100) {
		//perfect upgrade
		if (factory->getAttack()->getAttackRange() <= MELEE_UNIT_RECOGNITION_RANGE && Tools::random(0, 2) != 1) {
			//upgrade def if melee unit and 66% chance
			//check if health is good for upgrade and upgrade
			bool upgraded = false;
			do {
				upgraded = false;
				//upgrade health unit
				//check if dps is good for upgrade and upgrade
				float healthPerGoldOnBuild = factory->getUnit()->getMaxHitpoints() / (float) factory->getAddCost();
				float oldHealth = factory->getUnit()->getMaxHitpoints();
				float newHealth = oldHealth * factory->getUnit()->getHitpointsUpgradeMultiplier();
				float healthPerGoldOnUpgrade = (newHealth - oldHealth) / factory->getHitPointsUpCost();
				healthPerGoldOnUpgrade *= factory->getSpawnNum();

				if (healthPerGoldOnBuild < healthPerGoldOnUpgrade) {
					upgraded = factory->upgradeHitpoints();
				}

				//upgrade armour
				if (Tools::random(0, 3) == 1 && factory->getArmourUpCost() < factory->getHitPointsUpCost())
					upgraded |= factory->upgradeArmour();

			} while (upgraded);

		} else {
			bool upgraded = false;
			do {
				upgraded = false;
				//upgrade dps unit
				//check if dps is good for upgrade and upgrade
				float dpsPerGoldOnBuild = factory->getAttack()->getDamagePerSecond() / (float) factory->getAddCost();
				float oldDPS = factory->getAttack()->getDamagePerSecond() * factory->getAttack()->getDamageMultiplier();
				float newDPS = oldDPS * factory->getAttack()->getUpgradeMultiplier();
				float dpsPerGoldOnUpgrade = (newDPS - oldDPS) / factory->getArmourUpCost();
				dpsPerGoldOnUpgrade *= factory->getSpawnNum();

				if (dpsPerGoldOnBuild < dpsPerGoldOnUpgrade) {
					upgraded = factory->upgradeAttack();
				}
			} while (upgraded);
		}
	} else {
		//bad upgrade
		if (factory->getSpawnNum() >= Tools::random(1, 3)) {
			if (Tools::random(0, 2) == 1)
				factory->upgradeAttack();
			if (Tools::random(0, 2) == 1)
				factory->upgradeArmour();
			if (Tools::random(0, 2) == 1)
				factory->upgradeHitpoints();

			if (Tools::random(0, 3) == 1)
				factory->upgradeSkill();
			if (Tools::random(0, 3) == 1)
				factory->upgradeMana();
			if (Tools::random(0, 3) == 1)
				factory->upgradeManaReg();
			if (Tools::random(0, 3) == 1)
				factory->upgradeRunSpeed();
		}
	}

	return factory->addOneUnit();
}

std::map<DamageType, float> PlayerNetworkAI::getToughnessOfMeleeUnits(Player* player) {
	std::map<DamageType, float> toughnessOfMeleeUnits;
	//init
	for (int currentDamageType = NORMAL; currentDamageType <= CHAOS; currentDamageType++) {
		toughnessOfMeleeUnits[(DamageType) currentDamageType] = 0;
	}

	//iterate all factories
	std::vector<UnitFactory*> allUnitFactories = player->getRace()->getAllUnitFactories();

	for (std::vector<UnitFactory*>::iterator uf = allUnitFactories.begin(); uf != allUnitFactories.end(); ++uf) {
		UnitFactory* factory = (*uf);
		Unit* unit = factory->getUnit();
		std::vector<DamageType> redution = unit->getArmour()->getDamageReductionType();

		//iterate damage reduction of unit
		for (std::vector<DamageType>::iterator reductionType = redution.begin(); reductionType != redution.end(); ++reductionType) {
			toughnessOfMeleeUnits[*reductionType] += unit->getMaxHitpoints() * factory->getSpawnNum();
		}
	}

	return toughnessOfMeleeUnits;
}

std::map<DamageType, float> PlayerNetworkAI::getDPSPerDamageType(Player* player) {
	std::map<DamageType, float> dpsPerDamageType;
	//init
	for (int currentDamageType = NORMAL; currentDamageType <= CHAOS; currentDamageType++) {
		dpsPerDamageType[(DamageType) currentDamageType] = 0;
	}

	std::vector<UnitFactory*> allUnitFactories = player->getRace()->getAllUnitFactories();

	for (std::vector<UnitFactory*>::iterator uf = allUnitFactories.begin(); uf != allUnitFactories.end(); ++uf) {
		UnitFactory* factory = (*uf);
		int num = factory->getNumOfCreations();
		float dps = factory->getUnit()->getAttack()->getDamagePerSecond();
		DamageType damageType = factory->getUnit()->getAttack()->getDamageType();

		dpsPerDamageType[damageType] += dps * (float) num;
	}

	return dpsPerDamageType;
}

std::vector<UnitFactory*> PlayerNetworkAI::getPossibleUnits(Player* player, UnitType type) {
	std::vector<UnitFactory*> possibleUnits;
	std::vector<UnitFactory*> allUnitFactories;
	Tier currentTier = player->getCurrentTier();
	if (currentTier >= Tier1) {
		int maxRandom = 0;
		if (currentTier == Tier3) {
			maxRandom += 3;
		}
		if (currentTier == Tier2) {
			maxRandom += 3;
		}
		if (Tools::random(0, maxRandom) == 0) {
			std::vector<UnitFactory*> tmp = player->getRace()->getUnitFactories(Tier1);
			allUnitFactories.insert(allUnitFactories.end(), tmp.begin(), tmp.end());
		}
	}
	if (currentTier >= Tier2) {
		int maxRandom = 0;
		if (currentTier == Tier3) {
			maxRandom += 3;
		}
		if (Tools::random(0, maxRandom) == 0) {
			std::vector<UnitFactory*> tmp = player->getRace()->getUnitFactories(Tier2);
			allUnitFactories.insert(allUnitFactories.end(), tmp.begin(), tmp.end());
		}
	}
	if (currentTier >= Tier3) {
		std::vector<UnitFactory*> tmp = player->getRace()->getUnitFactories(Tier3);
		allUnitFactories.insert(allUnitFactories.end(), tmp.begin(), tmp.end());
	}

	for (std::vector<UnitFactory*>::iterator uf = allUnitFactories.begin(); uf != allUnitFactories.end(); ++uf) {
		UnitFactory* factory = (*uf);
		Unit* unit = factory->getUnit();
		switch (type) {
		case TANK:
			if (unit->getAttack()->getAttackRange() <= MELEE_UNIT_RECOGNITION_RANGE) {
				possibleUnits.push_back(factory);
			}
			break;
		case DPS:
			if (unit->getAttack()->getAttackRange() > MELEE_UNIT_RECOGNITION_RANGE) {
				possibleUnits.push_back(factory);
			}
			break;
		case SUPPORT:
			//support
			if (unit->getAttack()->isHeal()) {
				possibleUnits.push_back(factory);
			}
			break;
		}
	}

	return possibleUnits;
}
