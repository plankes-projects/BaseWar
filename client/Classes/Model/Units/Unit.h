/*
 * Unit.h
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#ifndef UNIT_H_
#define UNIT_H_

#include "../Attacks/Attack.h"
#include "../../View/BW_Sprite.h"
#include "../ArmyType.h"
#include "../Armours/Armour.h"
#include "../DamageType.h"
#include "../Skills/Skill.h"
#include "../Effects/Effect.h"
#include <algorithm>
#include <vector>

class Unit: public BW_Sprite {
public:
	virtual ~Unit();
	Unit(std::string image, float height, float hitpoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana, float manaRegPerSeconds,
			bool fadeIn = true);
	float getMaxHitpoints();
	void setMaxHitpoints(float maxHitpoints);
	Attack* getAttack();
	void setAttack(Attack* attack);
	ArmyType getArmyType();
	void setArmyType(ArmyType armyType);
	float getHitpoints();
	void setHitpoints(float hitpoints);
	void kill();
	void remove();
	void update();
	void doSomethingIntelligent();
	float Receivedamage(float damage, DamageType damagetype);
	float recieveHeal(float heal, DamageType healType);
	double getLastAttack();
	float getXAxisProgess();
	void setLastAttack(double lastAttack);
	virtual void moveTo(BW_Point location);
	bool hasEffect(Effect* effect);

	/**
	 * removes a harmful effect from the unit.
	 * If removed returns GUID of effect, else 0
	 */
	int removeADebuff();

	/**
	 * removes a positive effect from the unit.
	 * If removed returns true, else false
	 */
	bool removeABuff();

	Armour* getArmour() {
		return _armour;
	}

	int getAddCost() {
		return _addCost;
	}

	std::string getUnitName() {
		return _unitName;
	}

	const char* getNormalButtonImage() {
		return _normalButtonImage;
	}

	const char* getSelectedButtonImage() {
		return _selectedButtonImage;
	}

	const char* getDisabledButtonImage() {
		return _disabledButtonImage;
	}

	virtual Unit* clone() = 0;

	bool isHealAble() {
		return _isHealAble;
	}

	virtual bool isMoveAble();

	std::list<Skill*> getSkills() {
		return _skills;
	}

	void applyEffect(Effect* effect);

	/**
	 * 1 = 100%
	 */
	void increaseAttackDamageBy(float increaseWithPercent) {
		_attack->increaseAttackDamageBy(increaseWithPercent);
	}

	void decreaseAttackDamageBy(float decreaseWithPercent) {
		_attack->decreaseAttackDamageBy(decreaseWithPercent);
	}

	void increaseAttackSpeedBy(float increaseWithPercent) {
		_attack->increaseAttackSpeedBy(increaseWithPercent);
		initAnimationVarsAttackSpeed();
		updateAttackAnimationSpeed(getProgressOfAttackLoopInPercent());
	}

	void decreaseAttackSpeedBy(float decreaseWithPercent) {
		_attack->decreaseAttackSpeedBy(decreaseWithPercent);
		initAnimationVarsAttackSpeed();
		updateAttackAnimationSpeed(getProgressOfAttackLoopInPercent());
	}

	void increaseArmourEffectBy(float increaseWithPercent) {
		_armour->increaseArmourEffectBy(increaseWithPercent);
	}

	void decreaseArmourEffectBy(float decreaseWithPercent) {
		_armour->decreaseArmourEffectBy(decreaseWithPercent);
	}

	void increaseRunSpeedBy(float increaseWithPercent) {
		_runspeedEffectMultiplier *= 1.0f + increaseWithPercent;
		initAnimationVarsRunSpeed();
		updateMoveAnimationSpeed();
	}

	void decreaseRunSpeedBy(float decreaseWithPercent) {
		_runspeedEffectMultiplier /= 1.0f + decreaseWithPercent;
		initAnimationVarsRunSpeed();
		updateMoveAnimationSpeed();
	}

	void increaseHitpointsBy(float percent) {
		setMaxHitpoints(getMaxHitpoints() * (1 + percent));
		setHitpoints(getHitpoints() * (1 + percent));
	}

	void decreaseHitpointsBy(float percent) {
		setMaxHitpoints(getMaxHitpoints() / (1 + percent));
		setHitpoints(getHitpoints() / (1 + percent));
	}

	float getRunspeed() {
		return _runspeed * _runspeedEffectMultiplier;
	}

	void addEffectImmunity(int effectGUID) {
		_immuneToEffectGUID.push_back(effectGUID);
	}

	void removeEffectImmunity(int effectGUID) {
		std::vector<int>::iterator position = std::find(_immuneToEffectGUID.begin(), _immuneToEffectGUID.end(), effectGUID);
		if (position != _immuneToEffectGUID.end())
			_immuneToEffectGUID.erase(position);
	}

	virtual bool isImmuneToEffect(Effect* effect) {
		std::vector<int>::iterator position = std::find(_immuneToEffectGUID.begin(), _immuneToEffectGUID.end(), effect->getGUID());
		return position != _immuneToEffectGUID.end();
	}

	bool _randomPosStart;
	float _roundsToLive;

	float getMana() {
		return _mana;
	}

	float getManaPerSeconds() {
		return _manaRegPerSeconds;
	}

	void reduceManaBy(float mana) {
		_mana -= mana;
		if (_mana < 0)
			_mana = 0;
		updateManaLabel(_mana / _maxMana);
	}

	float getHitpointsUpgradeMultiplier() {
		return _hitPointsUpgradeMultiplier;
	}

	void upgradeHitpoints() {
		_hitpoints *= _hitPointsUpgradeMultiplier;
		_maxHitpoints *= _hitPointsUpgradeMultiplier;
	}

	void upgradeRunSpeed() {
		_runspeed *= _runSpeedUpgradeMultiplier;
	}

	void upgradeSkills();

	void upgradeMana() {
		_maxMana *= _manaUpgradeMultiplier;
		_mana *= _manaUpgradeMultiplier; //in the case someone missed to use max hitpoitns at clones
	}

	void upgradeManaReg() {
		_manaRegPerSeconds *= _manaRegUpgradeMultiplier;
	}

	bool isAttacked() {
		return _isAttacked;
	}

	void multiplayReceivedHealBy(float percent) {
		_healRecievingMultiplicator *= percent;
	}

	void devideReceivedHealBy(float percent) {
		_healRecievingMultiplicator /= percent;
	}

	float getProgressOfAttackLoopInPercent();
	void applyHandicap(float multiplicator);
	void turnIntoUltimate();
	void turnIntoMaster();

private:
	void updateMana();
	void initAnimationVars();
	void initAnimationVarsAttackSpeed();
	void initAnimationVarsRunSpeed();
	// dmg of attack is done at end of attack animation,
	// with this sync var we are able to move the point of dmg to a previous point
	double _attackAnimationAttackSync;
	void updateEffects();
	float _lastCheckedHP;
	bool _isAttacked;

protected:
	std::list<Skill*> cloneSkills(std::list<Skill*> skills);

	float _hitPointsUpgradeMultiplier;
	float _runSpeedUpgradeMultiplier;
	float _runspeedEffectMultiplier;
	float _manaUpgradeMultiplier;
	float _manaRegUpgradeMultiplier;

	const char* _normalButtonImage;
	const char* _selectedButtonImage;
	const char* _disabledButtonImage;

	bool _isHealAble;
	std::list<Skill*> _skills;
	std::list<Effect*> _effects;
	std::vector<int> _immuneToEffectGUID;
	Attack* _attack;
	Armour* _armour;

	//ArmyType armyType; // is set by Army.addunit()
	float _hitpoints;
	float _maxHitpoints;
	float _runspeed;
	ArmyType _armyType;
	double _lastAttack; // to watch cooldown of attack (1/attackSpeed)
	virtual void onDie() = 0;
	Unit* getNearestEnemyUnit();
	Unit* getHealTarget();
	bool attack(Unit* unit);
	int _addCost;

	std::string _unitName;
	bool _inRemoveList;
	bool _turnAbleOnAttack;

	float _maxMana;
	float _mana;
	float _manaRegPerSeconds;
	float _healRecievingMultiplicator;
	double _lastManaUpdate;

	double _created;

	std::string _attackEffectSound;
};

#endif /* UNIT_H_ */
