/*
 * Attack.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "Attack.h"
#include "../Model.h"
#include "../../Tools/BW_Time.h"

Attack::Attack(float damagePerSecond, DamageType damageType, float attackSpeed, float attackRange) {
	_upgradeNum = 0;
	_upgradeMultiplier = 1.15; //standard value
	_upgradeRangeMultiplier = 1.15;

	this->_attackSpeed = attackSpeed;
	this->_attackRange = attackRange;
	this->_damageType = damageType;
	this->_isHeal = false;

	//reformat to damage per second
	setDamagePerSecond(damagePerSecond); //ise this because we translate from damage per second to dmg

	_attackDamageEffectMultiplier = 1;
	_attackSpeedEffectMultiplier = 1;
}

Attack::~Attack() {
	// TODO Auto-generated destructor stub
}

void Attack::cloneUpgradeStatsFrom(Attack* attack) {
	_upgradeMultiplier = attack->getUpgradeMultiplier();
	_upgradeNum = attack->getUpgradeNum();
}

float Attack::getDamageMultiplier() {
	return (pow(_upgradeMultiplier, _upgradeNum)) * _attackDamageEffectMultiplier;
}

bool Attack::isHeal() {
	return _isHeal;
}

float Attack::getDamagePerSecond() {
	return this->_damage * this->_attackSpeed;
}

void Attack::setDamagePerSecond(float damageType) {
	this->_damage = damageType / _attackSpeed;
}

float Attack::getDamage() {
	return this->_damage;
}

void Attack::setDamage(float damage) {
	this->_damage = damage;
}

float Attack::getAttackSpeed() {
	return this->_attackSpeed * _attackSpeedEffectMultiplier;
}

float Attack::getAttackRange() {
	return this->_attackRange;
}

void Attack::setAttackRange(float attackRange) {
	this->_attackRange = attackRange;
}

ArmyType Attack::getArmyType() {
	return this->_armyType;
}

void Attack::setArmyType(ArmyType armyType) {
	this->_armyType = armyType;
}

DamageType Attack::getDamageType() {
	return _damageType;
}
void Attack::setDamageType(DamageType damageType) {
	_damageType = damageType;
}

bool Attack::tryAttack(Unit* attacker, Unit* target) {
	float timeBetweenAttacks = (1 / getAttackSpeed()) * 1000; // to get
	// miliseconds

	double now = BW_Time::getMilliSecondsCached();
	// diff < 0 because of overflow handling of now
	double diff = now - attacker->getLastAttack();
	if (diff < 0 || diff < timeBetweenAttacks) {
		return false;
	}
	attacker->setLastAttack(now);
	BAM(attacker, target);
	return true;
}

void Attack::upgrade() {
	_upgradeNum++;
}
