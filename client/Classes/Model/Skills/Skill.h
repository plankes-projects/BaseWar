/*
 * Attack.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef Skill_H_
#define Skill_H_

class Unit;
#include <string>

class Skill {
public:
	virtual ~Skill();
	Skill(float manaCost, float rechargeTime);
	void tryPerform(Unit* owner);
	virtual Skill* clone() = 0;
	std::string getInfo();

	int getUpgradeNum() {
		return _upgradeNum;
	}

	void upgrade() {
		_upgradeNum++;
		upgradeSkill();
	}

	virtual void turnIntoMaster() {
		//overwrite if u want to change something at the skill
	}

	virtual void turnIntoUltimate() {
		//overwrite if u want to change something at the skill
	}

protected:
	virtual void upgradeSkill() = 0;
	virtual std::string getSkillInfo() = 0;
	/**
	 * Returns true if performed, else false
	 */
	virtual bool perform(Unit* owner) = 0;
	float _rechargeTime; //in milli seconds
	bool _ownerIsAttacked;
	float _manaCost;
	float _upgradeMultiplier;

private:
	double _lastperform;
	float _lastCheckedHP;
	int _upgradeNum;
};

#endif /* ATTACK_H_ */
