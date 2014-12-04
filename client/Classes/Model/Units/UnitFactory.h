/*
 * UnitFactory.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef UNITFACTORY_H_
#define UNITFACTORY_H_

#include "../ArmyType.h"
#include "../Player.h"
#include "MoveAbleUnit.h"
#include "Tier.h"

#include "cocos2d.h"
USING_NS_CC;
//this is a CCNode, so we are able to use it at the button! =D

class UnitFactory: public cocos2d::CCNode {
public:
	UnitFactory(MoveAbleUnit* unit);
	virtual ~UnitFactory();
	virtual void createUnits();
	void setArmyType(ArmyType armyType);
	ArmyType getArmyType();
	int getAddCost();
	void setOwner(Player* player);

	/**
	 * returning false if something went wrong (not enough money)
	 */
	bool updateWithgameStat(GameStat gamestat, bool onlySell);
	void syncGameStat(GameStat* gamestat);

	bool isAddAble();
	bool addOneUnit();
	void addOneUnitTransit(CCObject* pSender);
	bool isSellAble();
	void sellOneUnitTransit(CCObject* pSender);
	bool sellOneUnit();

	bool isArmourUpgradeAble();
	void upgradeArmourTransit(CCObject* pSender);
	bool upgradeArmour();

	bool isAttackUpgradeAble();
	void upgradeAttackTransit(CCObject* pSender);
	bool upgradeAttack();
	float getAttackUpCost();
	int getAttackUpNum();

	bool isSkillUpgradeAble();
	void upgradeSkillTransit(CCObject* pSender);
	bool upgradeSkill();
	float getSkillUpCost();
	int getSkillUpNum();

	bool isManaUpgradeAble();
	void upgradeManaTransit(CCObject* pSender);
	bool upgradeMana();
	float getManaUpCost();
	int getManaUpNum();

	bool isManaRegUpgradeAble();
	void upgradeManaRegTransit(CCObject* pSender);
	bool upgradeManaReg();
	float getManaRegUpCost();
	int getManaRegUpNum();

	bool isRangeUpgradeAble();
	void upgradeRangeTransit(CCObject* pSender);
	bool upgradeRange();
	float getRangeUpCost();
	int getRangeUpNum();

	bool isHitPointsUpgradeAble();
	void upgradeHitpointsTransit(CCObject* pSender);
	bool upgradeHitpoints();
	bool isRunSpeedUpgradeAble();
	void upgradeRunSpeedTransit(CCObject* pSender);
	bool upgradeRunSpeed();

	const char* getDisabledButtonImage();
	const char* getNormalButtonImage();
	const char* getSelectedButtonImage();

	float getArmourUpCost();
	float getHitPointsUpCost();
	float getRunSpeedUpCost();

	int getArmourUpNum();
	int getHitPointsUpNum();
	int getRunSpeedUpNum();

	int getSpawnNum();

	Tier getTier();
	void setTier(Tier tier);

	float getSellPrice();

	Player* getOwner();

	std::string getUnitName();

	inline Attack* getAttack() {
		return _unit->getAttack();
	}

	inline float getHitpoints() {
		return _unit->getMaxHitpoints();
	}

	inline float getMana() {
		return _unit->getMana();
	}

	inline float getManaPerSeconds() {
		return _unit->getManaPerSeconds();
	}

	inline Armour* getArmour() {
		return _unit->getArmour();
	}

	inline float getRunspeed() {
		return _unit->getRunspeed();
	}

	inline Unit* getUnit() {
		return _unit;
	}

	void setId(int id);
	int getId();

	std::list<Skill*> getSkills() {
		return _unit->getSkills();
	}

	void applyUnitHandicap(float multiplicator);

	void increaseNumOfCreationsNoGoldUsed(){
		_numOfCreations++;
	}

	void resetnumOfCreation(){
		_numOfCreations = 0;
	}

	int getNumOfCreations(){
		return _numOfCreations;
	}

protected:
	GameStat _myGameStat;
	int _id;

	ArmyType _armyType;
	int _numOfCreations;

	float _armourUpgradeCost;
	int _armourUpgradeCostStep;
	int _armourUpgradedNum;
	int _armourUpgradesPerTier;

	float _attackUpgradeCost;
	int _attackUpgradeCostStep;
	int _attackUpgradedNum;
	int _attackUpgradesPerTier;

	float _runSpeedUpgradeCost;
	int _runSpeedUpgradeCostStep;
	int _runSpeedUpgradedNum;
	int _runSpeedUpgradesPerTier;

	float _hitPointsUpgradeCost;
	int _hitPointsUpgradeCostStep;
	int _hitPointsUpgradedNum;
	int _hitPointaUpgradesPerTier;

	float _skillUpgradeCost;
	int _skillUpgradeCostStep;
	int _skillUpgradedNum;
	int _skillUpgradesPerTier;

	float _manaUpgradeCost;
	int _manaUpgradeCostStep;
	int _manaUpgradedNum;
	int _manaUpgradesPerTier;

	float _manaRegUpgradeCost;
	int _manaRegUpgradeCostStep;
	int _manaRegUpgradedNum;
	int _manaRegUpgradesPerTier;

	float _rangeUpgradeCost;
	int _rangeUpgradeCostStep;
	int _rangeUpgradedNum;
	int _rangeUpgradesPerTier;

	int _spawned; //for statistics at end of game

	Player* _owner; //is set at Player::addFactory
	Tier _tier; //this is set at Race::addFactory

	Unit* _unit;
};

#endif /* UNITFACTORY_H_ */
