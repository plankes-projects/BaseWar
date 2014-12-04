/*
 * Race.h
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#ifndef RACECHAOS_H_
#define RACECHAOS_H_

#include "UnitFactory.h"
#include "Race.h"

class RaceChaos: public Race {
public:
	void spawn();
	RaceChaos();
	void setArmyTypeAndOwner(ArmyType armyType, Player* owner);
	void applyRaceHandicap(float multiplicator);

private:
	~RaceChaos();
	std::vector<UnitFactory*> _possibleUnitFactory;
};

#endif /* RACECHAOS_H_ */
