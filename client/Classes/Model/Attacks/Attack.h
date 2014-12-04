/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef ATTACK_H_
#define ATTACK_H_

#include "../ArmyType.h"
#include "../DamageType.h"
#include "../../Constants.h"
#include <string>
class Unit;

class Attack {
public:
	virtual ~Attack();
	Attack(float damagePerSecond, DamageType damageType, float attackSpeed, float attackRange);
	float getDamage();
	void setDamage(float damage);
	float getDamagePerSecond();
	void setDamagePerSecond(float damagePerSecond);
	float getAttackSpeed();
	float getAttackRange();
	void setAttackRange(float attackRange);
	float getFlightSpeed();
	void setGenerated(double generated);
	ArmyType getArmyType();
	void setArmyType(ArmyType armyType);
	DamageType getDamageType();
	void setDamageType(DamageType damageType);
	bool tryAttack(Unit* attacker, Unit* target);
	bool isHeal();

	void upgrade();

	virtual std::string getAttackInfo() = 0;

	float getUpgradeMultiplier() {
		return _upgradeMultiplier;
	}

	int getUpgradeNum() {
		return _upgradeNum;
	}

	virtual Attack* clone() = 0;
	void cloneUpgradeStatsFrom(Attack* attack);

	void increaseAttackDamageBy(float increaseWithPercent) {
		_attackDamageEffectMultiplier += increaseWithPercent;
	}

	void decreaseAttackDamageBy(float decreaseWithPercent) {
		_attackDamageEffectMultiplier -= decreaseWithPercent;
	}

	void increaseAttackSpeedBy(float increaseWithPercent) {
		_attackSpeedEffectMultiplier += increaseWithPercent;
	}

	void decreaseAttackSpeedBy(float decreaseWithPercent) {
		_attackSpeedEffectMultiplier -= decreaseWithPercent;
	}
	float getDamageMultiplier();

	void upgradeRange() {
		_attackRange *= _upgradeRangeMultiplier;
	}

	void applyHandicap(float multiplicator){
		_damage *= multiplicator;
	}

	virtual void turnIntoUltimate(){
		//do not forget projectile attack
		_damage *= UNITS_TO_ULTIMATE_UNIT_MULT;
	}

	virtual void turnIntoMaster(){
		//do not forget projectile attack
		_damage *= UNITS_TO_MASTER_UNIT_MULT;
	}

private:

	float _attackDamageEffectMultiplier;
	float _attackSpeedEffectMultiplier;

protected:
	float _damage; // dmg per attack
	float _attackSpeed; //attacks per second
	float _upgradeRangeMultiplier;
	float _attackRange;
	ArmyType _armyType; // is set by Unit.setArmyType()
	DamageType _damageType;
	int _upgradeNum;
	float _upgradeMultiplier;
	bool _isHeal; //false at healers for example

	/**
	 * target found for the attack and cooldown ready.
	 * target ready to recieve the hammer, BAM!
	 */
	void virtual BAM(Unit* attacker, Unit* target) = 0;

	//virtual void onHit(Unit* hitTarget) = 0;
};

#endif /* ATTACK_H_ */
