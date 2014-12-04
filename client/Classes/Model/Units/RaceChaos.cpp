/*
 * Race.cpp
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#include "RaceChaos.h"
#include "../../Tools/Tools.h"

RaceChaos::RaceChaos() :
		Race() {
	_isSkipUnitInfo = true;
	_AIOnly = true;
	setRaceButton0("chaosRace0.png");
	setRaceButton1("chaosRace1.png");

	//get all units
	for (int raceId = 1;; raceId++) {
		Race* race = createRaceWithId(raceId);
		if (race == NULL)
			break;

		for (int tier = Tier1; tier <= Tier3; tier++) {
			std::vector<UnitFactory*> facts = race->getUnitFactories((Tier) tier);
			for (std::vector<UnitFactory*>::iterator uf = facts.begin(); uf != facts.end(); ++uf) {
				UnitFactory* f = *uf;
				//increase number by one because we want to spawn one unit per createUnit call
				_possibleUnitFactory.push_back(f);
			}
		}

		race->setDeleteUnitFactoriesInDestructor(false);
		delete race;

	}
}

RaceChaos::~RaceChaos() {
	for (std::vector<UnitFactory*>::iterator uf = _possibleUnitFactory.begin(); uf != _possibleUnitFactory.end(); ++uf) {
		delete (*uf);
	}
}

void RaceChaos::setArmyTypeAndOwner(ArmyType armyType, Player* owner) {
	for (std::vector<UnitFactory*>::iterator it = _possibleUnitFactory.begin(); it != _possibleUnitFactory.end(); ++it) {
		(*it)->setArmyType(armyType);
		(*it)->setOwner(owner);
	}
}

void RaceChaos::applyRaceHandicap(float multiplicator) {
	multiplicator *= 0.75; //otherwise this is too hard

	for (std::vector<UnitFactory*>::iterator it = _possibleUnitFactory.begin(); it != _possibleUnitFactory.end(); ++it) {
		(*it)->applyUnitHandicap(multiplicator);
	}
}

void RaceChaos::spawn() {
	UnitFactory* firstUnitFactory = _possibleUnitFactory[0];
	int availableGold = firstUnitFactory->getOwner()->getGold() * 0.8; // *0.8 because it is not win able otherwise

	int buildFails = 0;
	int maxBuildFails = 10;

	while (buildFails < maxBuildFails) {
		int index = Tools::random(0, _possibleUnitFactory.size() - 1);
		UnitFactory* f = _possibleUnitFactory[index];
		if (f->getAddCost() < availableGold) {
			f->increaseNumOfCreationsNoGoldUsed();
			availableGold -= f->getAddCost();
		} else {
			buildFails++;
		}
	}

	for (std::vector<UnitFactory*>::iterator it = _possibleUnitFactory.begin(); it != _possibleUnitFactory.end(); ++it) {
		(*it)->createUnits();
		(*it)->resetnumOfCreation();
	}

}
