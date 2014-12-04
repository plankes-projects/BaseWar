/*
 * GameStat.h
 *
 *  Created on: 20.07.2013
 *      Author: Planke
 */

#ifndef PlayerNetworkAI_H_
#define PlayerNetworkAI_H_

#include <string>
#include <map>
#include "DamageType.h"
#include "ArmyType.h"
#include "Units/UnitFactory.h"

class Player;
class PlayerNetworkAI {
public:
	PlayerNetworkAI(float strength);
	virtual ~PlayerNetworkAI();
	void performFor(Player* player);

private:
	enum UnitType {
		TANK, DPS, SUPPORT
	};

	std::map<DamageType, float> getDPSPerDamageType(Player* player);

	/**
	 * DamageType is the armourtype reduction
	 * float is the healthpoints
	 */
	std::map<DamageType, float> getToughnessOfMeleeUnits(Player* player);

	/**
	 * returns false if there should be an upgrade but failed
	 */
	bool upgradeTier(Player* player, bool force = false);
	void upgradeGold(Player* player, bool force = false);
	void buildUnitDispatcher(Player* player, Player* enemy);

	void buildTank(Player* player, Player* enemy);
	void buildDPS(Player* player, Player* enemy);
	void buildSupport(Player* player, Player* enemy);

	/**
	 *	return false if there was not enough gold to do the task
	 */
	bool buildUnitOrUpgrade(UnitFactory* factory);

	/**
	 *	gets all possible unitfactories
	 *	@param type 0=tank, 1=dps, 2=support
	 */
	std::vector<UnitFactory*> getPossibleUnits(Player* player, UnitType type);

	/**
	 * how good the AI is building from 0 to 1
	 */
	float _strength;

};

#endif /* PlayerNetworkAI_H_ */
