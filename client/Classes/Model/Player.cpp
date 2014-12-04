/*
 * Player.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "Player.h"
#include "Units/UnitFactory.h"
#include "../Tools/BW_Time.h"
#include "../Tools/Tools.h"
#include "Model.h"
#include "../Network/NetworkSingleton.h"
#include "../Constants.h"
#include "PlayerNetworkAI.h"

Player::Player(std::string name, Race* race, bool AI, bool network) {
	_name = name;
	_AI = AI;
	_networkPlayer = network;
	_created = BW_Time::getMilliSecondsCached();
	_lastUpdate = BW_Time::getMilliSecondsCached();
	_race = race;
	_currentTier = Tier1;
	_forceOldAI = false;
	_wasShownAsAIInNetworkGame = false;
	_fakeNetworkPlayerStrength = ((float) Tools::random(5, 10)) / 10.0f; //something betwee 0.5 and 1

	_gold = 200; //start gold
	if (Model::getInstance()->getGameMode() == TUTORIAL) {
		_gold = 450; //to buy the whole first tier and speed things up at tutorial
	}
	_goldPeak = _goldAbsolut = _gold;

	_income = 4; //gold per seconds without upgrade
	_additionalIncome = 0;
	if (_AI) {
		switch (Model::getInstance()->getDifficulty()) {
		case DIF_EASY:
			//no change
			CCLOG("created easy player");
			_race->applyRaceHandicap(0.70);
			break;
		case DIF_NORMAL:
			CCLOG("created normal player");
			//we have changes at tactic here
			break;
		case DIF_HARD:
			CCLOG("created hard player");
			_race->applyRaceHandicap(1.2);
			break;
		case DIF_VERYHARD:
			CCLOG("created very hard player");
			_race->applyRaceHandicap(1.4);
			break;
		case DIF_HELL:
			CCLOG("created hell player");
			_race->applyRaceHandicap(1.8);
			break;
		default:
			CCLOG("ERROR: INVALID DIFFICULTY @ player.cpp!");
		}
	}

	if (_race->isAIOnly() && !_AI) {
		//someone tries to hack this game.
		//maybe from internet?
		//handle this gracefully
		delete _race;
		_race = Race::createRaceWithId(0); //randomized race
		_AI = true;
	}

	_incomeUpdateNum = 0; //current upgrade number
	_incomeUpdateCost = 100; //upgrade cost for first upgrade
	_incomeUpdateCostStep = 2; //upgrade multiplier for upgrade
	_incomeUpdatesPerTier = 50; //upgrade per tier
}
void Player::changeNetworkPlayerToAIPlayer() {
	if (_AI)
		return;

	CCLOG("Changing network player to AI player!");
	_AI = true;
	_networkPlayer = false;
	removeNetworkPlayerGoldBuffers();
}

void Player::changeToForceOldAIPlayer() {
	changeNetworkPlayerToAIPlayer();
	_forceOldAI = true;
}

std::string Player::getNetworkDisplayName() {
	return getName() + (showAsAIInNetworkGame() ? " - AI" : "");
}

bool Player::showAsAIInNetworkGame() {
	//handle fake network game name ---------------------------
	if (_forceOldAI)
		return true;

	if (NetworkSingleton::getInstance()->fakeNetworkGame)
		return _forceOldAI;

	//handle "real" network game name -------------------------
	if (_wasShownAsAIInNetworkGame)
		return true;

	if (_AI) {
		int sec = BW_Time::getMilliSecondsOverCached() / 1000 + 1;
		float min = sec / 60.0f;

		if (min <= 5)
			_wasShownAsAIInNetworkGame = true;
	}

	return _wasShownAsAIInNetworkGame;
}

bool Player::isAI() {
	return _AI;
}

void Player::updateWithgameStat(GameStat gameStat) {
	bool onlySell = false;
	CCLOG("gamestat: %s", gameStat.toString().c_str());
	while (_incomeUpdateNum < gameStat.getGoldUp()) {
		CCLOG("Upgrade income");
		if (!upgradeIncome()) {
			onlySell = true;
			break;
		}
	}

	//upgrade tier
	int tier = gameStat.getPlayerTier();
	if (!onlySell && (_currentTier == Tier1 && (tier == Tier2 || tier == Tier3))) {
		CCLOG("Upgrade to tier2");
		if (!upgradeToTier2())
			onlySell = true;
	}

	if (!onlySell && (_currentTier == Tier2 && tier == Tier3)) {
		CCLOG("Upgrade to tier3");
		if (!upgradeToTier3())
			onlySell = true;
	}

	//sync factories
	std::vector<UnitFactory*> unitFactories;

	//sync tier1
	unitFactories = _race->getUnitFactories(Tier1);
	for (std::vector<UnitFactory*>::iterator it = unitFactories.begin(); it != unitFactories.end(); ++it) {
		if (!(*it)->updateWithgameStat(gameStat, onlySell)) {
			onlySell = true;
			break;
		}
	}

	//sync tier 2
	if (_currentTier == Tier2 || _currentTier == Tier3) {
		unitFactories = _race->getUnitFactories(Tier2);
		for (std::vector<UnitFactory*>::iterator it = unitFactories.begin(); it != unitFactories.end(); ++it) {
			if (!(*it)->updateWithgameStat(gameStat, onlySell)) {
				onlySell = true;
				break;
			}
		}
	}

	//sync tier 3
	if (_currentTier == Tier3) {
		unitFactories = _race->getUnitFactories(Tier3);
		for (std::vector<UnitFactory*>::iterator it = unitFactories.begin(); it != unitFactories.end(); ++it) {
			if (!(*it)->updateWithgameStat(gameStat, onlySell)) {
				onlySell = true;
				break;
			}
		}
	}
}

float Player::getincomeUpgradeCost() {
	return _incomeUpdateCost;
}

bool Player::isIncomeUpgradeAble() {
	if (_gold < _incomeUpdateCost)
		return false;

	//anz possible upgrades
	int poss = getCurrentTier() * _incomeUpdatesPerTier; //tier 1 is int 1, tier 2 int 2 ...
	if (_incomeUpdateNum < poss)
		return true;

	return false;
}

void Player::performAIActions() {
	//called before spawn
	int randNum = Tools::random(0, 400) + _goldAbsolut; //0-120 + secondsofGame at 4 gold per seconds

	if (_currentTier == Tier3 || randNum > 4000) {
		//build tier 3 units
		AIBuildRandomTierUnits(Tier3);
		AITryToUpgradeUnitWithMaxNumber(Tier3, 2);
		CCLOG("AI build Tier3");
	} else if (_currentTier == Tier2 || randNum > 2000) {
		//rand max 100 + 360 = maxRand + 360sec
		//--> first can occor 6 min after start
		//build tier 2 units
		AIBuildRandomTierUnits(Tier2);
		AITryToUpgradeUnitWithMaxNumber(Tier2, 3);
		CCLOG("AI build Tier2");
	} else {
		bool iControlMiddle = Model::getInstance()->getMiddleControl() == _armyType;
		if (iControlMiddle && Model::getInstance()->getDifficulty() != DIF_EASY) {
			upgradeIncome();
		}

		//build tier 1 units
		AIBuildRandomTierUnits(Tier1);
		AITryToUpgradeUnitWithMaxNumber(Tier1, 4);
		CCLOG("AI build Tier1");
	}
}

void Player::AITryToUpgradeUnitWithMaxNumber(Tier tier, int minNumberOfUnit) {
	if (Model::getInstance()->getDifficulty() == DIF_EASY)
		return; //we do not upgrade if computer is set to easy

	UnitFactory* bestFactory = 0;
	int bestSpawnNum = 0;

	std::vector<UnitFactory*> unitFacts = _race->getUnitFactories(tier);
	for (std::vector<UnitFactory*>::iterator it = unitFacts.begin(); it != unitFacts.end(); ++it) {
		int currentSpawnNum = (*it)->getSpawnNum();
		if (currentSpawnNum >= minNumberOfUnit && currentSpawnNum > bestSpawnNum) {
			bestSpawnNum = currentSpawnNum;
			bestFactory = (*it);
		}
	}

	if (bestFactory != 0) {
		CCLOG("AI upgrades units.");
		if (Tools::random(0, 2) == 1)
			bestFactory->upgradeAttack();
		if (Tools::random(0, 2) == 1)
			bestFactory->upgradeArmour();
		if (Tools::random(0, 2) == 1)
			bestFactory->upgradeHitpoints();
		if (Tools::random(0, 3) == 1 && (Model::getInstance()->getDifficulty() == DIF_EASY || Model::getInstance()->getDifficulty() == DIF_NORMAL))
			bestFactory->upgradeRunSpeed(); //only upgrade runspeed if we have easy or normal computer

		if (tier != Tier1 && Model::getInstance()->getDifficulty() != DIF_EASY && Model::getInstance()->getDifficulty() != DIF_NORMAL) {
			//upgrade mana stuff too
			if (Tools::random(0, 3) == 1)
				bestFactory->upgradeSkill();
			if (Tools::random(0, 3) == 1)
				bestFactory->upgradeMana();
			if (Tools::random(0, 3) == 1)
				bestFactory->upgradeManaReg();
		}
	}
}

void Player::AIBuildRandomTierUnits(Tier tier) {
	if (tier == Tier3) {
		upgradeToTier2();
		upgradeToTier3();
	} else if (tier == Tier2)
		upgradeToTier2();

	std::vector<UnitFactory*> unitFacts = _race->getUnitFactories(tier);

	bool con = false;
	do {
		int randNum = Tools::random(0, unitFacts.size() - 1);

		int i = 0;
		for (std::vector<UnitFactory*>::iterator it = unitFacts.begin(); it != unitFacts.end(); ++it) {
			if (i == randNum) {
				con = (*it)->addOneUnit();
				break;
			}
			i++;
		}
	} while (con);
}

void Player::upgradeIncomeTransit(CCObject* pSender) {
	upgradeIncome();
}

bool Player::upgradeIncome() {
	if (!isIncomeUpgradeAble())
		return false;

	decreaseGoldBy(_incomeUpdateCost);
	_incomeUpdateCost *= _incomeUpdateCostStep;
	_incomeUpdateNum++;

	if (this == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setGoldUp(_incomeUpdateNum);
	return true;
}

Player::~Player() {
	delete _race;
}

void Player::spawnUnits() {
	//if AI only we have our own implementation of AI in race spawn
	if (_AI && !_race->isAIOnly()) {
		if (Model::getInstance()->getStartedAs() == NETWORK && NetworkSingleton::getInstance()->fakeNetworkGame && !_forceOldAI) {
			PlayerNetworkAI ai = PlayerNetworkAI(_fakeNetworkPlayerStrength);
			ai.performFor(this);
		} else {
			performAIActions();
		}
	}
	_race->spawn();
}

Race* Player::getRace() {
	return _race;
}

bool Player::isToTier2Able() {
	return (_currentTier == Tier1 && _race->getTier2Cost() <= _gold);
}

float Player::getTier2Cost() {
	return _race->getTier2Cost();
}
float Player::getTier3Cost() {
	return _race->getTier3Cost();
}

bool Player::upgradeToTier2() {
	if (!isToTier2Able())
		return false;

	decreaseGoldBy(_race->getTier2Cost());
	_currentTier = Tier2;

	if (this == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setPlayerTier(_currentTier);
	return true;
}

bool Player::isToTier3Able() {
	return (_currentTier == Tier2 && _race->getTier3Cost() <= _gold);
}

bool Player::upgradeToTier3() {
	if (!isToTier3Able())
		return false;

	decreaseGoldBy(_race->getTier3Cost());
	_currentTier = Tier3;

	if (this == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setPlayerTier(_currentTier);
	return true;
}

void Player::setArmyTyp(ArmyType armyType) {
	this->_armyType = armyType;
	_race->setArmyTypeAndOwner(armyType, this);
}

ArmyType Player::getArmyTyp() {
	return _armyType;
}

void Player::update() {
	double now = BW_Time::getMilliSecondsCached();
	// diff < 0 because of overflow handling of now
	double timeDelay = now - _lastUpdate;
	_lastUpdate = now;
	if (timeDelay < 0)
		return;

	//calculate how much money this player gets per second
	float increaseBy = getGoldPerSeconds() * timeDelay / 1000.0f;
	increaseBy += _additionalIncome * timeDelay / 1000.0f;
	increaseGoldBy(increaseBy);

	float decreaseBy = TIER_PRICE_REDUCTION_PER_SECOND * timeDelay / 1000.0f;
	_race->reduceTier2Cost(decreaseBy);
	_race->reduceTier3Cost(decreaseBy);
}

float Player::getGoldPerSeconds() {
	float multiplier = 1;
	if (Model::getInstance()->getMiddleControl() == _armyType)
		multiplier *= GOLD_FOR_MIDDLE_CONTROL;

	multiplier *= pow(GOLD_FOR_GOLD_UPGRADE, _incomeUpdateNum); //15% per upgrade
	return _income * multiplier;
}

float Player::getGold() {
	return _gold;
}

Tier Player::getCurrentTier() {
	return _currentTier;
}

void Player::setCurrentTier(Tier currentTier) {
	_currentTier = currentTier;
}

void Player::applyNetworkPlayerGoldBuffers() {
	increaseGoldBy(GOLD_ON_TOWER_KILL); //prevent tower kill async
	_income *= GOLD_FOR_GOLD_UPGRADE; //prevent middle control async
}

void Player::removeNetworkPlayerGoldBuffers() {
	float goldFromIncomeBuffer = _goldAbsolut - (_goldAbsolut / GOLD_FOR_GOLD_UPGRADE);
	decreaseGoldBy(GOLD_ON_TOWER_KILL + goldFromIncomeBuffer);
	_income /= GOLD_FOR_GOLD_UPGRADE; //prevent middle control async
}

GameStat Player::getGameStat() {
	GameStat gs = GameStat();

	gs.setGoldUp(_incomeUpdateNum);
	gs.setPlayerTier(_currentTier);

	//sync factories
	std::vector<UnitFactory*> unitFactories = _race->getAllUnitFactories();
	for (std::vector<UnitFactory*>::iterator it = unitFactories.begin(); it != unitFactories.end(); ++it) {
		(*it)->syncGameStat(&gs);
	}

	return gs;
}
