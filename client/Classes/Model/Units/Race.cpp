/*
 * Race.cpp
 *
 *  Created on: May 21, 2013
 *      Author: planke
 */

#include "Race.h"
#include "Baron.h"
#include "Troll.h"
#include "Viking.h"
#include "Archer.h"
#include "ModernSoldier.h"
#include "NinjaMelee.h"
#include "NinjaRange.h"
#include "Darksaber.h"
#include "Minion.h"
#include "Robot.h"
#include "Pinkie.h"
#include "Soldier.h"
#include "Tank.h"
#include "FlareAntlion.h"
#include "FlareSkelet.h"
#include "FlareTroll.h"

#include "BotanicArcher.h"
#include "BotanicDaemon.h"
#include "BotanicDog.h"
#include "BotanicFemmage.h"
#include "BotanicKnight.h"
#include "BotanicMage.h"
#include "BotanicPriest.h"
#include "Sheep.h"
#include "Golem.h"
#include "Defector.h"
#include "Slime.h"

#include "../Attacks/InstantAttack.h"
#include "../Attacks/InstantHeal.h"
#include "../Attacks/CleaveAttack.h"
#include "../Attacks/ProjectileAttack.h"
#include "../Attacks/SuizideAttack.h"
#include "../Attacks/LifeLeechAttack.h"

#include "../Attacks/PoisonCleaveAttack.h"

#include "../Attacks/Projectiles/ArrowProjectile.h"
#include "../Attacks/Projectiles/FieryBallProjectile.h"
#include "../Attacks/Projectiles/FireBallProjectile.h"
#include "../Attacks/Projectiles/NormalProjectile.h"
#include "../Attacks/Projectiles/SplashBombProjectile.h"
#include "../Attacks/Projectiles/IceProjectile.h"

#include "../Armours/NormalArmour.h"
#include "../Armours/LightArmour.h"
#include "../Armours/HeavyArmour.h"
#include "../Armours/UnarmouredArmour.h"

#include "UnitFactory.h"
#include "../Skills/TeleportAttack.h"
#include "../Skills/TeleportEscape.h"
#include "../Skills/RageSkill.h"
#include "../Skills/RemoveBuffSkill.h"
#include "../Skills/RemoveDebuffSkill.h"
#include "../Skills/LastStandSkill.h"
#include "../Skills/DespairSkill.h"
#include "../Skills/ManaBurnSkill.h"
#include "../Skills/ReduceReceivedHealSkill.h"

#include "../../Tools/Tools.h"
#include "RaceChaos.h"

Race::Race() {
	_deleteUnitFactoriesInDestructor = true;
	_AIOnly = false;
	_isSkipUnitInfo = false;
	_tier2Cost = 200; //standard values
	_tier3Cost = 400;

	_raceId = 0; //init
	_randomRaceString = ""; //set at race id 0
}

char Race::_randomRaceStringConnector = '-';
int Race::_randomRaceUnitsPerTier = 3;

Race::~Race() {
	if (_deleteUnitFactoriesInDestructor) {
		for (std::vector<UnitFactory*>::iterator uf = _tier1.begin(); uf != _tier1.end(); ++uf) {
			delete (*uf);
		}
		for (std::vector<UnitFactory*>::iterator uf = _tier2.begin(); uf != _tier2.end(); ++uf) {
			delete (*uf);
		}
		for (std::vector<UnitFactory*>::iterator uf = _tier3.begin(); uf != _tier3.end(); ++uf) {
			delete (*uf);
		}
	}
}

float Race::getTier2Cost() {
	return _tier2Cost;
}

float Race::getTier3Cost() {
	return _tier3Cost;
}

void Race::reduceTier2Cost(float reduce) {
	_tier2Cost -= reduce;
	if (_tier2Cost < 0)
		_tier2Cost = 0;
}

void Race::reduceTier3Cost(float reduce) {
	_tier3Cost -= reduce;
	if (_tier3Cost < 0)
		_tier3Cost = 0;
}

void Race::addUnitFactory(Tier tier, UnitFactory* unitFactory) {
	unitFactory->setTier(tier);
	switch (tier) {
	case Tier1:
		unitFactory->setId(_tier1.size());
		_tier1.push_back(unitFactory);
		return;
	case Tier2:
		unitFactory->setId(_tier2.size());
		_tier2.push_back(unitFactory);
		return;
	case Tier3:
		unitFactory->setId(_tier3.size());
		_tier3.push_back(unitFactory);
		return;
	}
	CCLOG("If you see this, we have a bug in Race.cpp!! Error: 123");
}
std::vector<UnitFactory*> Race::getUnitFactories(Tier tier) {
	switch (tier) {
	case Tier1:
		return _tier1;
	case Tier2:
		return _tier2;
	case Tier3:
		return _tier3;
	}
	CCLOG("If you see this, we have a bug in Race.cpp!! Error: 124");
	return _tier1;
}

std::vector<UnitFactory*> Race::getAllUnitFactories() {
	std::vector<UnitFactory*> ret;

	ret.insert(ret.end(), _tier1.begin(), _tier1.end());
	ret.insert(ret.end(), _tier2.begin(), _tier2.end());
	ret.insert(ret.end(), _tier3.begin(), _tier3.end());

	return ret;
}

Race* Race::createRaceWithId(int id) {

	Race* r = 0;
	switch (id) {
	case 0:
		r = buildRaceFromRandomizedRaceString(createRandomizedRaceString());
		break;
	case 1:
		r = createMindChamberRace();
		break;
	case 2:
		r = createBotanicRace();
		break;
	case 3:
		r = createThirdRace();
		break;

		// 100+ are computer races only
	case START_ID_FOR_AI_RACES:
		r = new RaceChaos();
		break;
	}

	if (r == 0)
		return 0;

	r->setRaceId(id);
	return r;
}

int Race::getRaceId() {
	return _raceId;
}

void Race::setRaceId(int id) {
	_raceId = id;
}

std::string Race::createRandomizedRaceString() {
	Race* createdRace = NULL;
	int raceId = 1;
	for (;; raceId++) {
		createdRace = createRaceWithId(raceId);
		if (createdRace == NULL)
			break;
		delete createdRace;
	}

	//on every tier we have 3 units.
	//so we now there are 3 * (raceId - 1) units per tier
	int allUnitsPerTier = 3 * (raceId - 1);
	std::string randomizedRaceString = "";
	std::string connector = "";
	if (allUnitsPerTier < 3) {
		CCLOG("BUG in Race.cpp Error code: 12354");
		return "";
	}

	for (int tier = Tier1; tier <= Tier3; tier++) {
		//now we have to add 3x unique random number from 0 to unitsPerTier
		std::list<int> uniqueInts = std::list<int>();
		for (int i = 0; i < _randomRaceUnitsPerTier; i++) {
			// 0 would be the first unit of first race
			// 3 would be the first unit of second race
			bool gotNewRandomUnit;
			int newRandomUnit;
			do {
				//search until we have unique values
				newRandomUnit = Tools::random(0, allUnitsPerTier - 1);
				gotNewRandomUnit = true;
				//search the int in the list
				for (std::list<int>::iterator it = uniqueInts.begin(); it != uniqueInts.end(); ++it) {
					if ((*it) == newRandomUnit) {
						gotNewRandomUnit = false;
						break;
					}
				}

				//CCLOG("RandomUnit: %i |  0 - %i ---- got it: %i", newRandomUnit, (unitsPerTier - 1), gotNewRandomUnit);
			} while (!gotNewRandomUnit);

			uniqueInts.push_back(newRandomUnit);
			randomizedRaceString += connector + Tools::toString(newRandomUnit);

			connector = _randomRaceStringConnector;
		}
		//CCLOG("Created one tier!");
	}
	return randomizedRaceString;
}

Race* Race::buildRaceFromRandomizedRaceString(std::string randomizedRaceString) {
	Race* randomRace = new Race();
	randomRace->setRaceButton0("pRandomunitRace1.png");
	randomRace->setRaceButton1("pRandomunitRace2.png");

	randomRace->setRandomRaceString(randomizedRaceString);
	//check if string is ok
	// it should have (_randomRaceUnitsPerTier * 3) - 1 x _randomRaceStringConnector
	unsigned int numberOfSegments = _randomRaceUnitsPerTier * 3;
	std::vector < std::string > segments = Tools::splitStringBy(randomizedRaceString, _randomRaceStringConnector);
	if (numberOfSegments != segments.size()) {
		CCLOG("BUG in Race.cpp. %i != %i Error code: 125643", (int) numberOfSegments, (int) segments.size());
		delete randomRace;
		return NULL;
	}

	//convert string list to int list
	std::vector<int> intSegments = std::vector<int>();
	for (std::vector<std::string>::iterator it = segments.begin(); it != segments.end(); ++it) {
		intSegments.push_back(atoi((*it).c_str()));
	}

	//generate all races
	std::vector<Race*> races = std::vector<Race*>();
	for (int raceId = 1;; raceId++) {
		Race* createdRace = createRaceWithId(raceId);
		if (createdRace == NULL)
			break;
		races.push_back(createdRace);
	}
	int allUnitsPerTier = 3 * races.size();

	//check for unique ints and if ints are ok
	int tier = Tier1;
	for (unsigned int i = 0; i < intSegments.size(); i += _randomRaceUnitsPerTier) {
		std::list<int> uniqueInts = std::list<int>();
		for (int j = 0; j < _randomRaceUnitsPerTier; j++) {
			int newInt = intSegments[i + j];
			for (std::list<int>::iterator it = uniqueInts.begin(); it != uniqueInts.end(); ++it) {
				if ((*it) == newInt || newInt < 0 || newInt >= allUnitsPerTier) {
					CCLOG("randomized string is not correct. error code: 1267589\n %s", randomizedRaceString.c_str());

					//free heap again
					for (std::vector<Race*>::iterator ra = races.begin(); ra != races.end(); ++ra) {
						delete (*ra);
					}
					delete randomRace;
					return NULL;
				}
			}
			uniqueInts.push_back(newInt);
			randomRace->addUnitFactoryFromRandomizedCORRECTNumber((Tier) tier, newInt, races);
		}
		tier++;
	}

	//free heap again
	for (std::vector<Race*>::iterator ra = races.begin(); ra != races.end(); ++ra) {
		delete (*ra);
	}
	//CCLOG("Correct: %s", randomizedRaceString.c_str());
	return randomRace;
}

void Race::addUnitFactoryFromRandomizedCORRECTNumber(Tier tier, int correctRandomizedRaceNumber, std::vector<Race*> races) {
	int raceVectorPos = correctRandomizedRaceNumber / _randomRaceUnitsPerTier;
	int unitPos = correctRandomizedRaceNumber % _randomRaceUnitsPerTier;

	Race* r = races[raceVectorPos];
	std::vector<UnitFactory*> ufs = r->getUnitFactories(tier);
	UnitFactory* uf = ufs[unitPos];

	MoveAbleUnit* unit = (MoveAbleUnit*) (uf->getUnit()->clone());
	addUnitFactory(tier, new UnitFactory(unit));
}

Race* Race::createThirdRace() {
	Race* race = new Race();
	race->setRaceButton0("pNinjaMelee0.png");
	race->setRaceButton1("pNinjaMelee1.png");

	std::list<Skill*> skillList = std::list<Skill*>();
	skillList.push_back(new LastStandSkill(30, 0, 0.5, 5000, 0.6, 0.6));
	race->addUnitFactory(Tier1, new UnitFactory(new FlareAntlion(100, 500, 300, new InstantAttack(20, NORMAL, 1.5, 100), new HeavyArmour(), skillList, 30, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new ReduceReceivedHealSkill(10, 0, 800, 10000, 0.5));
	race->addUnitFactory(Tier1, new UnitFactory(new FlareTroll(125, 500, 150, new InstantAttack(60, NORMAL, 2, 200), new NormalArmour(), skillList, 10, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new RemoveBuffSkill(25, 0, 1000));
	race->addUnitFactory(Tier1,
			new UnitFactory(new Slime(150, 500, 450, new PoisonCleaveAttack(5000, 250, 3, 15, MAGIC, 1.5, 50), new NormalArmour(), skillList, 25, 1)));

	//tier 2---------------------------------

	skillList = std::list<Skill*>();
	skillList.push_back(new TeleportAttack(10, 0, 600));
	race->addUnitFactory(Tier2,
			new UnitFactory(new NinjaMelee(200, 500, 450, new InstantAttack(80, NORMAL, 1.5, 125), new UnarmouredArmour(), skillList, 10, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new RemoveDebuffSkill(15, 0, 600, 10000));
	race->addUnitFactory(Tier2, new UnitFactory(new Darksaber(250, 500, 1000, new InstantAttack(50, NORMAL, 1.5, 125), new HeavyArmour(), skillList, 25, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new TeleportEscape(10, 0, 500, 0.5));
	race->addUnitFactory(Tier2,
			new UnitFactory(
					new NinjaRange(250, 500, 300, new ProjectileAttack(new ProjectileFactory(new FieryBallProjectile()), 80, 2, 400), new UnarmouredArmour(),
							skillList, 10, 1)));

	//tier 3---------------------------------

	skillList = std::list<Skill*>();
	skillList.push_back(new TeleportAttack(10, 0, 600));
	race->addUnitFactory(Tier3, new UnitFactory(new Defector(425, 500, 1000, new InstantAttack(300, NORMAL, 2, 125), new LightArmour(), skillList, 10, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new RageSkill(10, 0, 400, 4000, 0, 0.5, 0.5));
	race->addUnitFactory(Tier3,
			new UnitFactory(new FlareSkelet(425, 500, 1500, new InstantAttack(200, NORMAL, 1.5, 100), new NormalArmour(), skillList, 10, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new RageSkill(10, 0, 1000, 4000, 0.25, 0.25, 0.25));
	race->addUnitFactory(Tier3,
			new UnitFactory(new ModernSoldier(450, 500, 300, new InstantAttack(200, MAGIC, 3, 700), new UnarmouredArmour(), skillList, 10, 1)));

	return race;
}

Race* Race::createMindChamberRace() {
	Race* race = new Race();
	race->setRaceButton0("pSoldier0.png");
	race->setRaceButton1("pSoldier1.png");

	race->addUnitFactory(Tier1,
			new UnitFactory(new Troll(100, 500, 450, new InstantAttack(20, NORMAL, 1, 125), new NormalArmour(), std::list<Skill*>(), 0, 0)));

	race->addUnitFactory(Tier1,
			new UnitFactory(new Viking(125, 500, 300, new CleaveAttack(250, 1, 0.5, 40, NORMAL, 1, 100), new LightArmour(), std::list<Skill*>(), 0, 0)));

	std::list<Skill*> skillList = std::list<Skill*>();
	skillList.push_back(new ReduceReceivedHealSkill(10, 0, 800, 10000, 0.5));
	race->addUnitFactory(Tier1,
			new UnitFactory(
					new Archer(150, 500, 150, new ProjectileAttack(new ProjectileFactory(new ArrowProjectile()), 60, 1, 500), new UnarmouredArmour(), skillList,
							10, 1)));

	//tier 2---------------------------------
	skillList = std::list<Skill*>();
	skillList.push_back(new RemoveDebuffSkill(15, 0, 1200, 10000));
	race->addUnitFactory(Tier2, new UnitFactory(new Pinkie(200, 500, 300, new InstantHeal(30, MAGIC, 4, 350), new UnarmouredArmour(), skillList, 25, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new RemoveBuffSkill(25, 0, 600));
	race->addUnitFactory(Tier2, new UnitFactory(new Minion(250, 500, 1000, new InstantAttack(50, NORMAL, 1.5, 150), new HeavyArmour(), skillList, 25, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new RageSkill(10, 0, 1000, 4000, 0.25, 0.25, 0.25));
	race->addUnitFactory(Tier2,
			new UnitFactory(
					new Soldier(275, 500, 300, new ProjectileAttack(new ProjectileFactory(new SplashBombProjectile(100, 0.5)), 40, 0.5, 600), new LightArmour(),
							skillList, 10, 1)));

	//tier 3---------------------------------

	skillList = std::list<Skill*>();
	skillList.push_back(new LastStandSkill(30, 0, 0.5, 5000, 0.6, 0.6));
	race->addUnitFactory(Tier3, new UnitFactory(new Robot(400, 300, 3500, new InstantAttack(40, NORMAL, 0.8, 125), new HeavyArmour(), skillList, 30, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new DespairSkill(20, 0, 10000, 0.3, 0.5, 0));
	race->addUnitFactory(Tier3,
			new UnitFactory(new Baron(425, 500, 1500, new CleaveAttack(250, 2, 0.5, 200, NORMAL, 2, 150), new LightArmour(), skillList, 20, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new RageSkill(10, 0, 1000, 4000, 0.25, 0.25, 0.25));
	race->addUnitFactory(Tier3,
			new UnitFactory(
					new Tank(450, 500, 1500, new ProjectileAttack(new ProjectileFactory(new SplashBombProjectile(100, 0.5)), 85, 0.5, 700), new NormalArmour(),
							skillList, 10, 1)));

	return race;
}

Race* Race::createBotanicRace() {
	Race* race = new Race();
	race->setRaceButton0("pBotanicDaemon0.png");
	race->setRaceButton1("pBotanicDaemon1.png");

	race->addUnitFactory(Tier1, new UnitFactory(new Sheep(100, 500, 125, new SuizideAttack(100, 175), new UnarmouredArmour(), std::list<Skill*>(), 0, 0)));

	race->addUnitFactory(Tier1,
			new UnitFactory(new BotanicDog(110, 500, 300, new CleaveAttack(250, 2, 0.9, 20, NORMAL, 1, 100), new LightArmour(), std::list<Skill*>(), 0, 0)));

	std::list<Skill*> skillList = std::list<Skill*>();
	skillList.push_back(new ReduceReceivedHealSkill(10, 0, 800, 10000, 0.5));
	race->addUnitFactory(Tier1,
			new UnitFactory(
					new BotanicArcher(150, 500, 150, new ProjectileAttack(new ProjectileFactory(new ArrowProjectile()), 60, 1, 500), new UnarmouredArmour(),
							skillList, 10, 1)));

	//tier 2---------------------------------
	skillList = std::list<Skill*>();
	skillList.push_back(new RemoveDebuffSkill(15, 0, 1200, 10000));
	race->addUnitFactory(Tier2,
			new UnitFactory(new BotanicPriest(200, 500, 300, new InstantHeal(30, MAGIC, 1, 350), new UnarmouredArmour(), skillList, 25, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new LastStandSkill(30, 0, 0.5, 5000, 0.6, 0.6));
	race->addUnitFactory(Tier2,
			new UnitFactory(new BotanicKnight(250, 500, 1000, new InstantAttack(50, NORMAL, 1.5, 150), new HeavyArmour(), skillList, 30, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new RemoveBuffSkill(25, 0, 1200));
	race->addUnitFactory(Tier2,
			new UnitFactory(
					new BotanicFemmage(250, 500, 300, new ProjectileAttack(new ProjectileFactory(new IceProjectile(0.5, 4000)), 60, 1, 600), new LightArmour(),
							skillList, 25, 1)));

	//tier 3---------------------------------
	skillList = std::list<Skill*>();
	skillList.push_back(new LastStandSkill(30, 0, 0.5, 5000, 0.6, 0.6));
	race->addUnitFactory(Tier3,
			new UnitFactory(new Golem(400, 300, 2200, new CleaveAttack(250, 4, 0.5, 75, NORMAL, 0.75, 125), new LightArmour(), skillList, 30, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new DespairSkill(20, 0, 10000, 0.3, 0.5, 0));
	race->addUnitFactory(Tier3,
			new UnitFactory(new BotanicDaemon(425, 500, 1500, new LifeLeechAttack(0.5, 180, NORMAL, 0.5, 175), new NormalArmour(), skillList, 30, 1)));

	skillList = std::list<Skill*>();
	skillList.push_back(new ManaBurnSkill(5, 0, 1200, 1));
	race->addUnitFactory(Tier3,
			new UnitFactory(
					new BotanicMage(450, 500, 300, new ProjectileAttack(new ProjectileFactory(new FireBallProjectile()), 250, 1.5, 650), new UnarmouredArmour(),
							skillList, 40, 1)));
	return race;
}

void Race::setArmyTypeAndOwner(ArmyType armyType, Player* owner) {
	for (std::vector<UnitFactory*>::iterator it = _tier1.begin(); it != _tier1.end(); ++it) {
		(*it)->setArmyType(armyType);
		(*it)->setOwner(owner);
	}

	for (std::vector<UnitFactory*>::iterator it = _tier2.begin(); it != _tier2.end(); ++it) {
		(*it)->setArmyType(armyType);
		(*it)->setOwner(owner);
	}

	for (std::vector<UnitFactory*>::iterator it = _tier3.begin(); it != _tier3.end(); ++it) {
		(*it)->setArmyType(armyType);
		(*it)->setOwner(owner);
	}
}

void Race::applyRaceHandicap(float multiplicator) {
	for (std::vector<UnitFactory*>::iterator it = _tier1.begin(); it != _tier1.end(); ++it) {
		(*it)->applyUnitHandicap(multiplicator);
	}

	for (std::vector<UnitFactory*>::iterator it = _tier2.begin(); it != _tier2.end(); ++it) {
		(*it)->applyUnitHandicap(multiplicator);
	}

	for (std::vector<UnitFactory*>::iterator it = _tier3.begin(); it != _tier3.end(); ++it) {
		(*it)->applyUnitHandicap(multiplicator);
	}
}

void Race::setRaceButton0(std::string name) {
	_raceButton0 = name;
}
void Race::setRaceButton1(std::string name) {
	_raceButton1 = name;
}

std::string Race::getRaceButton0() {
	return _raceButton0;
}
std::string Race::getRaceButton1() {
	return _raceButton1;
}

void Race::spawn() {

	std::vector<UnitFactory*> factories = _tier1;
	for (std::vector<UnitFactory*>::iterator it = factories.begin(); it != factories.end(); ++it)
		(*it)->createUnits();

	factories = _tier2;
	for (std::vector<UnitFactory*>::iterator it = factories.begin(); it != factories.end(); ++it)
		(*it)->createUnits();

	factories = _tier3;
	for (std::vector<UnitFactory*>::iterator it = factories.begin(); it != factories.end(); ++it)
		(*it)->createUnits();
}
