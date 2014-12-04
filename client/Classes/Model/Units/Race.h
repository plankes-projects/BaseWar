/*
 * Race.h
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#ifndef RACE_H_
#define RACE_H_

#include "Tier.h"
#include "../ArmyType.h"
#include <string>
#include <list>
#include <vector>

class Player;
class UnitFactory;
class Race {
public:
	std::vector<UnitFactory*> getUnitFactories(Tier tier);
	std::vector<UnitFactory*> getAllUnitFactories();

	//after creation, you need to call setArmyTypeAndOwner!
	static Race* createRaceWithId(int id);

	static Race* createMindChamberRace();
	static Race* createBotanicRace();
	static Race* createThirdRace();

	/*
	 * This will look at all units possible.
	 * Than chose 3 from each tier and build a race with them.
	 */
	static std::string createRandomizedRaceString();
	static Race* buildRaceFromRandomizedRaceString(std::string randomizedRaceString);

	virtual void setArmyTypeAndOwner(ArmyType armyType, Player* owner);
	virtual ~Race();
	float getTier2Cost();
	float getTier3Cost();
	void reduceTier2Cost(float reduce);
	void reduceTier3Cost(float reduce);

	void setRaceButton0(std::string name);
	void setRaceButton1(std::string name);

	std::string getRaceButton0();
	std::string getRaceButton1();

	int getRaceId();
	void setRaceId(int id);

	std::string getRandomRaceString() {
		return _randomRaceString;
	}

	void setRandomRaceString(std::string randomRaceString) {
		_randomRaceString = randomRaceString;
	}

	virtual void applyRaceHandicap(float multiplicator);

	virtual void spawn();

	bool isSkipUnitInfo(){
		return _isSkipUnitInfo;
	}

	bool isAIOnly(){
		return _AIOnly;
	}

	void setDeleteUnitFactoriesInDestructor(bool enable){
		_deleteUnitFactoriesInDestructor = enable;
	}

protected:
	bool _isSkipUnitInfo;
	/**
	 * If we set AIOnly we have to implement the AI ourself in the subclass
	 */
	bool _AIOnly;
	Race();
	bool _deleteUnitFactoriesInDestructor;

private:
	std::string _randomRaceString;
	static char _randomRaceStringConnector;
	static int _randomRaceUnitsPerTier;
	std::string _name;
	std::string _raceButton0;
	std::string _raceButton1;
	std::vector<UnitFactory*> _tier1;
	std::vector<UnitFactory*> _tier2;
	std::vector<UnitFactory*> _tier3;

	float _tier2Cost;
	float _tier3Cost;
	int _raceId;


	void addUnitFactory(Tier tier, UnitFactory* unitFactory);
	void addUnitFactoryFromRandomizedCORRECTNumber(Tier tier, int correctRandomizedRaceNumber, std::vector<Race*> races);
};

#endif /* RACE_H_ */
