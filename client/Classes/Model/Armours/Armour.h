/*
 * Armour.h
 *
 *  Created on: May 20, 2013
 *      Author: planke
 */

#ifndef ARMOUR_H_
#define ARMOUR_H_

#include "../DamageType.h"
#include <string>
#include <vector>
class Unit;

class Armour {
public:
	Armour();
	virtual ~Armour();
	virtual float realDamage(float damage, DamageType damageType);
	virtual float realHeal(float heal, DamageType healType) = 0;
	virtual std::string getArmourInfo() = 0;
	virtual void update(Unit* owner);
	void upgrade();
	float getUpgrademultiplier() {
		return _upgradeMultiplier;
	}

	int getUpgradeNum() {
		return _upgradeNum;
	}
	virtual Armour* clone() = 0;
	void cloneUpgradeStatsFrom(Armour* armour);
	void increaseArmourEffectBy(float increaseWithPercent) {
		_armourEffectMultiplier *= 1.0f + increaseWithPercent;
	}

	void decreaseArmourEffectBy(float decreaseWithPercent) {
		_armourEffectMultiplier /= 1.0f + decreaseWithPercent;
	}

	std::vector<DamageType> getDamageReductionType(){
		return _damageReduction;
	}

protected:
	float getArmourMultiplier();
	int _upgradeNum;
	float _upgradeMultiplier;
	float _armourEffectMultiplier;
	std::vector<DamageType> _damageReduction;
};

#endif /* ARMOUR_H_ */
