/*
 * Player.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <list>
#include "ArmyType.h"
#include "Units/Race.h"
#include "Units/Tier.h"
#include "../Network/GameStat.h"

class UnitFactory;
#include "cocos2d.h"
using namespace cocos2d;
//this is a CCNode, so we are able to use it at the button! =D
class Player: public cocos2d::CCNode {
public:
	Player(std::string name, Race* race, bool AI, bool network);
	virtual ~Player();
	void setArmyTyp(ArmyType armyType); //is set by Army::addPlayer
	ArmyType getArmyTyp();
	void spawnUnits();
	Race* getRace();
	void update();
	float getGold();
	void increaseGoldBy(float gold) {
		_gold += gold;
		_goldAbsolut += gold;
		if (_gold > _goldPeak)
			_goldPeak = _gold;
	}
	void decreaseGoldBy(float gold) {
		_gold -= gold;
	}

	float getGoldPeak() {
		return _goldPeak;
	}

	Tier getCurrentTier();
	void setCurrentTier(Tier currentTier);

	bool isToTier2Able();
	bool isToTier3Able();

	bool upgradeToTier2();
	bool upgradeToTier3();

	bool isIncomeUpgradeAble();
	void upgradeIncomeTransit(CCObject* pSender);
	bool upgradeIncome();

	void performAIActions();

	float getGoldPerSeconds();

	float getTier2Cost();
	float getTier3Cost();
	float getincomeUpgradeCost();

	std::string getName() {
		return _name;
	}

	void updateWithgameStat(GameStat gameStat);
	void changeNetworkPlayerToAIPlayer();
	void changeToForceOldAIPlayer();
	bool isAI();

	int _networkPlayerId;
	bool _networkPlayer;

	std::string getNetworkDisplayName();

	void applyNetworkPlayerGoldBuffers();
	void removeNetworkPlayerGoldBuffers();
	float getGoldAbsolute() {
		return _goldAbsolut;
	}

	GameStat getGameStat();

private:
	bool showAsAIInNetworkGame();
	void AIBuildRandomTierUnits(Tier tier);
	void AITryToUpgradeUnitWithMaxNumber(Tier tier, int minNumberOfUnit);
	bool _forceOldAI;
	bool _wasShownAsAIInNetworkGame;
	std::string _name;
	bool _AI; //computer if true
	ArmyType _armyType;
	float _gold;
	float _goldAbsolut; // the gold of the player over the whole game
	float _goldPeak;
	float _income;
	float _additionalIncome;	//used AI, this income is not effected by income upgrade
	int _incomeUpdateNum;
	int _incomeUpdateCost;
	int _incomeUpdateCostStep;
	int _incomeUpdatesPerTier;

	double _created;

	double _lastUpdate;
	Race* _race;
	Tier _currentTier;
	float _fakeNetworkPlayerStrength;
};

#endif /* PLAYER_H_ */
