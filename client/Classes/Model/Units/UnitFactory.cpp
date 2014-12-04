/*
 * UnitFactory.cpp
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#include "UnitFactory.h"
#include <stddef.h>
#include "../Model.h"
#include "../../Network/NetworkSingleton.h"

UnitFactory::UnitFactory(MoveAbleUnit* unit) {
	_myGameStat = GameStat();
	_unit = unit;

	_numOfCreations = 0;
	_spawned = 0;
	_owner = NULL;

	_armourUpgradeCost = 50;
	_armourUpgradeCostStep = 2;
	_armourUpgradedNum = 0;
	_armourUpgradesPerTier = 50;

	_attackUpgradeCost = 50;
	_attackUpgradeCostStep = 2;
	_attackUpgradedNum = 0;
	_attackUpgradesPerTier = 50;

	_runSpeedUpgradeCost = 50;
	_runSpeedUpgradeCostStep = 2;
	_runSpeedUpgradedNum = 0;
	_runSpeedUpgradesPerTier = 50;

	_hitPointsUpgradeCost = 50;
	_hitPointsUpgradeCostStep = 2;
	_hitPointsUpgradedNum = 0;
	_hitPointaUpgradesPerTier = 50;

	_skillUpgradeCost = 50;
	_skillUpgradeCostStep = 2;
	_skillUpgradedNum = 0;
	_skillUpgradesPerTier = 50;

	_manaUpgradeCost = 50;
	_manaUpgradeCostStep = 2;
	_manaUpgradedNum = 0;
	_manaUpgradesPerTier = 50;

	_manaRegUpgradeCost = 50;
	_manaRegUpgradeCostStep = 2;
	_manaRegUpgradedNum = 0;
	_manaRegUpgradesPerTier = 50;

	_rangeUpgradeCost = 50;
	_rangeUpgradeCostStep = 2;
	_rangeUpgradedNum = 0;
	_rangeUpgradesPerTier = 50;
}

void UnitFactory::syncGameStat(GameStat* gamestat) {
	if (_numOfCreations != 0)
		gamestat->setUnitNum(_tier, _id, _numOfCreations);

	if (getArmourUpNum() != 0)
		gamestat->setArmourUp(_tier, _id, getArmourUpNum());
	if (getAttackUpNum() != 0)
		gamestat->setAttackUp(_tier, _id, getAttackUpNum());
	if (getRunSpeedUpNum() != 0)
		gamestat->setRunspeedUp(_tier, _id, getRunSpeedUpNum());
	if (getHitPointsUpNum() != 0)
		gamestat->setLifeUp(_tier, _id, getHitPointsUpNum());

	if (getSkillUpNum() != 0)
		gamestat->setSkillUp(_tier, _id, getSkillUpNum());
	if (getManaUpNum() != 0)
		gamestat->setManaUp(_tier, _id, getManaUpNum());
	if (getManaRegUpNum() != 0)
		gamestat->setManaRegUp(_tier, _id, getManaRegUpNum());
	if (getRangeUpNum() != 0)
		gamestat->setRangeUp(_tier, _id, getRangeUpNum());
}

bool UnitFactory::updateWithgameStat(GameStat gameStat, bool onlySell) {
	_myGameStat = gameStat;
	int up = gameStat.getUnitNum(_tier, _id);

	while (_numOfCreations > up) {
		CCLOG("selling one unit %i > %i", _numOfCreations, up);
		if (!sellOneUnit())
			break;
	}

	if (onlySell)
		return true;

	while (_numOfCreations < up) {
		CCLOG("Adding one unit");
		if (!addOneUnit())
			return false;
	}

	up = gameStat.getArmourUp(_tier, _id);
	while (getArmourUpNum() < up) {
		CCLOG("Upgrade armour");
		if (!upgradeArmour())
			return false;
	}

	up = gameStat.getAttackUp(_tier, _id);
	while (getAttackUpNum() < up) {
		CCLOG("Upgrade attack");
		if (!upgradeAttack())
			return false;
	}

	up = gameStat.getRunspeedUp(_tier, _id);
	while (getRunSpeedUpNum() < up) {
		CCLOG("Upgrade speed");
		if (!upgradeRunSpeed())
			return false;
	}

	up = gameStat.getLifeUp(_tier, _id);
	while (getHitPointsUpNum() < up) {
		CCLOG("Upgrade hitpoints");
		if (!upgradeHitpoints())
			return false;
	}

	up = gameStat.getSkillUp(_tier, _id);
	while (getSkillUpNum() < up) {
		CCLOG("Upgrade skills");
		if (!upgradeSkill())
			return false;
	}

	up = gameStat.getManaUp(_tier, _id);
	while (getManaUpNum() < up) {
		CCLOG("Upgrade mana");
		if (!upgradeMana())
			return false;
	}

	up = gameStat.getManaRegUp(_tier, _id);
	while (getManaRegUpNum() < up) {
		CCLOG("Upgrade manareg");
		if (!upgradeManaReg())
			return false;
	}

	up = gameStat.getRangeUp(_tier, _id);
	while (getRangeUpNum() < up) {
		CCLOG("Upgrade range");
		if (!upgradeRange())
			return false;
	}

	return true;
}

void UnitFactory::setId(int id) {
	_id = id;
}
int UnitFactory::getId() {
	return _id;
}

int UnitFactory::getSpawnNum() {
	return _numOfCreations;
}

Player* UnitFactory::getOwner() {
	return _owner;
}

bool UnitFactory::isHitPointsUpgradeAble() {
	if (_owner == NULL || _owner->getGold() < _hitPointsUpgradeCost)
		return false;

	//anz possible upgrades
	int poss = (_owner->getCurrentTier() - _tier + 1) * _hitPointaUpgradesPerTier;
	if (_hitPointsUpgradedNum < poss)
		return true;

	return false;
}

bool UnitFactory::isRunSpeedUpgradeAble() {
	if (_owner == NULL || _owner->getGold() < _runSpeedUpgradeCost)
		return false;

	//anz possible upgrades
	int poss = (_owner->getCurrentTier() - _tier + 1) * _runSpeedUpgradesPerTier;
	if (_runSpeedUpgradedNum < poss)
		return true;

	return false;
}

bool UnitFactory::isArmourUpgradeAble() {
	if (_owner == NULL || _owner->getGold() < _armourUpgradeCost)
		return false;

	//anz possible upgrades
	int poss = (_owner->getCurrentTier() - _tier + 1) * _armourUpgradesPerTier;
	if (_armourUpgradedNum < poss)
		return true;

	return false;
}
bool UnitFactory::isAttackUpgradeAble() {
	if (_owner == NULL || _owner->getGold() < _attackUpgradeCost)
		return false;

	//anz possible upgrades
	int poss = (_owner->getCurrentTier() - _tier + 1) * _attackUpgradesPerTier;
	if (_attackUpgradedNum < poss)
		return true;

	return false;
}

void UnitFactory::upgradeHitpointsTransit(CCObject* pSender) {
	upgradeHitpoints();
}
bool UnitFactory::upgradeHitpoints() {
	if (!isHitPointsUpgradeAble())
		return false;
	_owner->decreaseGoldBy(_hitPointsUpgradeCost);
	_hitPointsUpgradeCost *= _hitPointsUpgradeCostStep;
	_hitPointsUpgradedNum++;

	_unit->upgradeHitpoints();

	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setLifeUp(_tier, _id, _hitPointsUpgradedNum);
	return true;
}

void UnitFactory::upgradeRunSpeedTransit(CCObject* pSender) {
	upgradeRunSpeed();
}

bool UnitFactory::upgradeRunSpeed() {
	if (!isRunSpeedUpgradeAble())
		return false;
	_owner->decreaseGoldBy(_runSpeedUpgradeCost);
	_runSpeedUpgradeCost *= _runSpeedUpgradeCostStep;
	_runSpeedUpgradedNum++;

	_unit->upgradeRunSpeed();

	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setRunspeedUp(_tier, _id, _runSpeedUpgradedNum);
	return true;
}

void UnitFactory::upgradeArmourTransit(CCObject* pSender) {
	upgradeArmour();
}

bool UnitFactory::upgradeArmour() {
	if (!isArmourUpgradeAble())
		return false;
	_unit->getArmour()->upgrade();
	_owner->decreaseGoldBy(_armourUpgradeCost);
	_armourUpgradeCost *= _armourUpgradeCostStep;
	_armourUpgradedNum++;
	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setArmourUp(_tier, _id, _armourUpgradedNum);
	return true;
}

void UnitFactory::upgradeAttackTransit(CCObject* pSender) {
	upgradeAttack();
}

bool UnitFactory::upgradeAttack() {
	if (!isAttackUpgradeAble())
		return false;
	_unit->getAttack()->upgrade();
	_owner->decreaseGoldBy(_attackUpgradeCost);
	_attackUpgradeCost *= _attackUpgradeCostStep;
	_attackUpgradedNum++;

	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setAttackUp(_tier, _id, _attackUpgradedNum);
	return true;
}

void UnitFactory::addOneUnitTransit(CCObject* pSender) {
	addOneUnit();
}

bool UnitFactory::addOneUnit() {
	if (!isAddAble())
		return false;

	_owner->decreaseGoldBy(_unit->getAddCost());
	_numOfCreations++;

	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setUnitNum(_tier, _id, _numOfCreations);
	return true;
}

bool UnitFactory::isAddAble() {
	return !(_owner == NULL || _owner->getGold() < _unit->getAddCost() || _tier > _owner->getCurrentTier());
}

float UnitFactory::getSellPrice() {
	switch (_tier) {
	case Tier1:
		return _unit->getAddCost() * 0.7;
	case Tier2:
		return _unit->getAddCost() * 0.5;
	case Tier3:
		return _unit->getAddCost() * 0.3;
	default:
		CCLOG("If you see this, we have a bug in UnitFactory.cpp!! Error: 1337");
		return 0;
	}
}

bool UnitFactory::isSellAble() {
	return !(_numOfCreations == 0 || _owner == NULL);
}

void UnitFactory::sellOneUnitTransit(CCObject* pSender) {
	sellOneUnit();
}

bool UnitFactory::sellOneUnit() {
	if (!isSellAble())
		return false;
	_owner->increaseGoldBy(getSellPrice());

	_numOfCreations--;

	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setUnitNum(_tier, _id, _numOfCreations);
	return true;
}

UnitFactory::~UnitFactory() {
	//do not delete owner, he is deleted by the race
	delete _unit;
}

void UnitFactory::setArmyType(ArmyType armyType) {
	_armyType = armyType;
}

ArmyType UnitFactory::getArmyType() {
	return _armyType;
}

int UnitFactory::getAddCost() {
	return _unit->getAddCost();
}
void UnitFactory::setOwner(Player* player) {
	_owner = player;
}

const char* UnitFactory::getDisabledButtonImage() {
	return _unit->getDisabledButtonImage();
}

const char* UnitFactory::getNormalButtonImage() {
	return _unit->getNormalButtonImage();
}

const char* UnitFactory::getSelectedButtonImage() {
	return _unit->getSelectedButtonImage();
}

Tier UnitFactory::getTier() {
	return _tier;
}

void UnitFactory::setTier(Tier tier) {
	this->_tier = tier;
}

float UnitFactory::getAttackUpCost() {
	return _attackUpgradeCost;
}

float UnitFactory::getArmourUpCost() {
	return _armourUpgradeCost;
}
float UnitFactory::getHitPointsUpCost() {
	return _hitPointsUpgradeCost;
}
float UnitFactory::getRunSpeedUpCost() {
	return _runSpeedUpgradeCost;
}

int UnitFactory::getAttackUpNum() {
	return _attackUpgradedNum;
}
int UnitFactory::getArmourUpNum() {
	return _armourUpgradedNum;
}
int UnitFactory::getHitPointsUpNum() {
	return _hitPointsUpgradedNum;
}
int UnitFactory::getRunSpeedUpNum() {
	return _runSpeedUpgradedNum;
}

std::string UnitFactory::getUnitName() {
	return _unit->getUnitName();
}

bool UnitFactory::isSkillUpgradeAble() {
	if (_owner == NULL || _owner->getGold() < _skillUpgradeCost)
		return false;

//anz possible upgrades
	int poss = (_owner->getCurrentTier() - _tier + 1) * _skillUpgradesPerTier;
	if (_skillUpgradedNum < poss)
		return true;

	return false;
}

void UnitFactory::upgradeSkillTransit(CCObject* pSender) {
	upgradeSkill();
}
bool UnitFactory::upgradeSkill() {
	if (!isSkillUpgradeAble())
		return false;
	_unit->upgradeSkills();
	_owner->decreaseGoldBy(_skillUpgradeCost);
	_skillUpgradeCost *= _skillUpgradeCostStep;
	_skillUpgradedNum++;

	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setSkillUp(_tier, _id, _skillUpgradedNum);
	return true;
}
float UnitFactory::getSkillUpCost() {
	return _skillUpgradeCost;
}
int UnitFactory::getSkillUpNum() {
	return _skillUpgradedNum;
}

bool UnitFactory::isManaUpgradeAble() {
	if (_owner == NULL || _owner->getGold() < _manaUpgradeCost)
		return false;

//anz possible upgrades
	int poss = (_owner->getCurrentTier() - _tier + 1) * _manaUpgradesPerTier;
	if (_manaUpgradedNum < poss)
		return true;

	return false;
}
void UnitFactory::upgradeManaTransit(CCObject* pSender) {
	upgradeMana();
}
bool UnitFactory::upgradeMana() {
	if (!isManaUpgradeAble())
		return false;
	_unit->upgradeMana();
	_owner->decreaseGoldBy(_manaUpgradeCost);
	_manaUpgradeCost *= _manaUpgradeCostStep;
	_manaUpgradedNum++;

	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setManaUp(_tier, _id, _manaUpgradedNum);
	return true;
}
float UnitFactory::getManaUpCost() {
	return _manaUpgradeCost;
}
int UnitFactory::getManaUpNum() {
	return _manaUpgradedNum;
}

bool UnitFactory::isRangeUpgradeAble() {
	if (_owner == NULL || _owner->getGold() < _rangeUpgradeCost)
		return false;

	//anz possible upgrades
	int poss = (_owner->getCurrentTier() - _tier + 1) * _rangeUpgradesPerTier;
	if (_rangeUpgradedNum < poss)
		return true;

	return false;
}

void UnitFactory::upgradeRangeTransit(CCObject* pSender) {
	upgradeRange();
}
bool UnitFactory::upgradeRange() {
	if (!isRangeUpgradeAble())
		return false;
	_unit->getAttack()->upgradeRange();
	_owner->decreaseGoldBy(_rangeUpgradeCost);
	_rangeUpgradeCost *= _rangeUpgradeCostStep;
	_rangeUpgradedNum++;

	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setRangeUp(_tier, _id, _rangeUpgradedNum);
	return true;

}
float UnitFactory::getRangeUpCost() {
	return _rangeUpgradeCost;
}
int UnitFactory::getRangeUpNum() {
	return _rangeUpgradedNum;
}

bool UnitFactory::isManaRegUpgradeAble() {
	if (_owner == NULL || _owner->getGold() < _manaRegUpgradeCost)
		return false;

//anz possible upgrades
	int poss = (_owner->getCurrentTier() - _tier + 1) * _manaRegUpgradesPerTier;
	if (_manaRegUpgradedNum < poss)
		return true;

	return false;
}
void UnitFactory::upgradeManaRegTransit(CCObject* pSender) {
	upgradeManaReg();
}
bool UnitFactory::upgradeManaReg() {
	if (!isManaRegUpgradeAble())
		return false;
	_unit->upgradeManaReg();
	_owner->decreaseGoldBy(_manaRegUpgradeCost);
	_manaRegUpgradeCost *= _manaRegUpgradeCostStep;
	_manaRegUpgradedNum++;

	if (this->_owner == Model::getInstance()->getThisPlayer())
		NetworkSingleton::getInstance()->_myGameStat.setManaRegUp(_tier, _id, _manaRegUpgradedNum);
	return true;
}
float UnitFactory::getManaRegUpCost() {
	return _manaRegUpgradeCost;
}
int UnitFactory::getManaRegUpNum() {
	return _manaRegUpgradedNum;
}

void UnitFactory::applyUnitHandicap(float multiplicator) {
	_unit->applyHandicap(multiplicator);
}

void UnitFactory::createUnits() {

	BW_Point p = Model::getInstance()->getMyArmy(_armyType)->getSpawnPoint();
	for (int i = 0; i < _numOfCreations; i++) {
		Unit* unit = _unit->clone();

		/*

		 //if u add this again, also add the game info message
		 if (_numOfCreations - i >= UNITS_TO_MASTER_UNIT) {
		 //handle master unit
		 i += UNITS_TO_MASTER_UNIT - 1; //-1 because we increment i at the end of this loop
		 unit->turnIntoMaster();
		 } else
		 */
		if (_numOfCreations - i >= UNITS_TO_ULTIMATE_UNIT) {
			//handle ultimate unit
			i += UNITS_TO_ULTIMATE_UNIT - 1; //-1 because we increment i at the end of this loop
			unit->turnIntoUltimate();
		}

		//sync spawn position

		if (Model::getInstance()->getGameMode() == NETWORK) {
			int y = NetworkSingleton::getInstance()->getNextSpawnPosition(_armyType);
			if (y >= 0 && y <= Model::getInstance()->getFieldHeigth()) {
				unit->setPosition(BW_Point(0, y));
				unit->_randomPosStart = false;
			}
		}

		Model::getInstance()->getMyArmy(_armyType)->addUnit(unit);
		Model::getInstance()->addAddedUnit(unit);	//for spawn alignment by attack range
		_spawned++;
	}
}
