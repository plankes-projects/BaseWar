/*
 * Unit.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "Unit.h"
#include "../Model.h"
#include "../../Tools/BW_Time.h"
#include "../../Tools/ViewTools.h"
#include "../../View/Animations/Blood.h"
#include "../../Sound/SoundHandler.h"
#include "../../Constants.h"

Unit::Unit(std::string image, float height, float hitpoints, Attack* attack, Armour* armour, std::list<Skill*> skills, float mana, float manaRegPerSeconds,
		bool fadeIn) :
		BW_Sprite(image, height, true, fadeIn) {

	_isUnit = true;

	//overwritten by sub classes
	_attackEffectSound = "sound_attack_bow";

	_healRecievingMultiplicator = 1;
	_runspeed = 0;
	_maxMana = _mana = mana;
	_manaRegPerSeconds = manaRegPerSeconds;
	_skills = skills;
	_armour = armour;
	_attack = attack;
	_effects = std::list<Effect*>();
	_immuneToEffectGUID = std::vector<int>();

	_runspeedEffectMultiplier = 1;

	if (skills.size() != 0)
		addManaBarToHealthBar();

	_lastCheckedHP = _maxHitpoints = hitpoints;
	_isAttacked = false;
	setHitpoints(hitpoints);
	_lastAttack = BW_Time::getMilliSecondsCached();
	_unitName = "NoName";
	_randomPosStart = true;
	_roundsToLive = 99999; //applied to base + tower, for moveable units, it will be overwritten
	_inRemoveList = false;
	_turnAbleOnAttack = true;
	_isHealAble = true;

	//standard values, overwrite possible
	_hitPointsUpgradeMultiplier = 1.15;
	_runSpeedUpgradeMultiplier = 1.15;

	_manaUpgradeMultiplier = 1.20;
	_manaRegUpgradeMultiplier = 1.20;

	_lastManaUpdate = _created = BW_Time::getMilliSecondsCached();
}

int Unit::removeADebuff() {
	for (std::list<Effect*>::iterator it = _effects.begin(); it != _effects.end(); ++it) {
		if ((*it)->isHarmful() && (*it)->forceRemove()) {
			return (*it)->getGUID();
		}
	}
	return 0;
}

bool Unit::removeABuff() {
	for (std::list<Effect*>::iterator it = _effects.begin(); it != _effects.end(); ++it) {
		if (!((*it)->isHarmful()) && (*it)->forceRemove()) {
			return true;
		}
	}
	return false;
}

bool Unit::hasEffect(Effect* effect) {
	for (std::list<Effect*>::iterator it = _effects.begin(); it != _effects.end(); ++it) {
		if (effect->equals(*it)) {
			return true;
		}
	}

	return false;
}

void Unit::applyEffect(Effect* effect) {
	if (isImmuneToEffect(effect))
		return;

	if (!effect->isStackAble()) {
		//search same effect
		for (std::list<Effect*>::iterator it = _effects.begin(); it != _effects.end(); ++it) {
			if (effect->equals(*it)) {
				(*it)->reApply(effect);
				return;
			}
		}
	}

	_effects.push_back(effect);
}

void Unit::upgradeSkills() {
	for (std::list<Skill*>::iterator it = _skills.begin(); it != _skills.end(); ++it) {
		(*it)->upgrade();
	}
}

std::list<Skill*> Unit::cloneSkills(std::list<Skill*> skills) {
	std::list<Skill*> skillsCloned = std::list<Skill*>();
	for (std::list<Skill*>::iterator it = skills.begin(); it != skills.end(); ++it) {
		skillsCloned.push_back((*it)->clone());
	}
	return skillsCloned;
}

bool Unit::isMoveAble() {
	return false;
}

Unit::~Unit() {
	while (!_skills.empty()) {
		delete _skills.front();
		_skills.pop_front();
	}

	while (!_effects.empty()) {
		delete _effects.front();
		_effects.pop_front();
	}

	delete _attack;
	delete _armour;
}

float Unit::getMaxHitpoints() {
	return this->_maxHitpoints;
}
void Unit::setMaxHitpoints(float maxHitpoints) {
	this->_maxHitpoints = maxHitpoints;
}
Attack* Unit::getAttack() {
	return this->_attack;
}
void Unit::setAttack(Attack* attack) {
	this->_attack = attack;
}

ArmyType Unit::getArmyType() {
	return this->_armyType;
}
void Unit::setArmyType(ArmyType armyType) {
	this->_armyType = armyType;
	this->_attack->setArmyType(armyType);
	setHealthPointIndicatorArmyType(armyType);
}
float Unit::getHitpoints() {
	return this->_hitpoints;
}
void Unit::setHitpoints(float hitpoints) {
	if (hitpoints > _maxHitpoints)
		hitpoints = _maxHitpoints;

	this->_hitpoints = hitpoints;
	updateHitpointLabel(_hitpoints / _maxHitpoints);
}

void Unit::kill() {
	onDie();
	remove();
}
void Unit::remove() {
	if (!_inRemoveList) {
		_inRemoveList = true;
		Model::getInstance()->addToRemoveUnit(this);
	}
}

void Unit::updateMana() {
	if (_skills.size() == 0)
		return; //no need to update, we are not able to use mana

	double now = BW_Time::getMilliSecondsCached();
	double timeDelay = now - _lastManaUpdate;
	_lastManaUpdate = now;
	if (timeDelay < 0)
		return;

	_mana += getManaPerSeconds() * timeDelay / 1000;
	if (_mana > _maxMana)
		_mana = _maxMana;

	updateManaLabel(_mana / _maxMana);
}

void Unit::update() {
	float life_time = BW_Time::getMilliSecondsCached() - _created;
	float max_life_time = _roundsToLive * Model::getInstance()->getSpawnSleep() * 1000; //to get milliseconds
	if(life_time > max_life_time){
		remove();
		return;	//unit will be removed, abort all other updates
	}
	updateLifeDurationIndicator(1 - (life_time / max_life_time));


	updateVisibleOfHealthBar();

	float lastCheckedHpTmp = _lastCheckedHP;
	_lastCheckedHP = getHitpoints();
	_isAttacked = lastCheckedHpTmp != _lastCheckedHP;

	updateMana();

	for (std::list<Skill*>::iterator it = _skills.begin(); it != _skills.end(); ++it) {
		(*it)->tryPerform(this);
	}

	updateEffects();

	_armour->update(this);
	doSomethingIntelligent();
}

void Unit::updateEffects() {
	std::list<Effect*> effectsToRemove = std::list<Effect*>();

	//update effects and collect effects to remove
	for (std::list<Effect*>::iterator it = _effects.begin(); it != _effects.end(); ++it) {

//		CCLOG("Effect loop 1");
		bool effectIsAlive = (*it)->tryPerform(this);
		//	CCLOG("Effect loop 2");
		if (!effectIsAlive) {
			effectsToRemove.push_back(*it); //remove effect
		}
	}

	//remove the collected effects
	Effect* effect;
	while (!effectsToRemove.empty()) {

		effect = effectsToRemove.front();
		effectsToRemove.pop_front();
		_effects.remove(effect);

		delete effect;
	}
}

void Unit::doSomethingIntelligent() {

	Unit* target = _attack->isHeal() ? this->getHealTarget() : this->getNearestEnemyUnit();

	if (target == NULL && _attack->isHeal())
		target = this->getNearestEnemyUnit();

	if (target == NULL) {
		this->doIdle();
		return;
	}

	if (!this->attack(target))
		this->moveTo(target->getPositionBW());
}

void Unit::moveTo(BW_Point location) {
	//this unit cant move, go idle
	this->doIdle();
}

float Unit::getXAxisProgess() {
	float width = Model::getInstance()->getFieldWidth();
	float x = _armyType == LEFT ? getPositionBW().x : width - getPositionBW().x;
	return x * 100 / width;
}

Unit* Unit::getHealTarget() {
	Unit* weakestUnit = NULL;
	float lowestHealth = 100; //in percent
	float health;

	Unit* mostProgess = NULL;
	float xProgess;
	float maxXProgess = 0;

	std::list<Unit*> units = Model::getInstance()->getMyArmy(_armyType)->getUnits();

	for (std::list<Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		//save lowest health unit
		health = (*it)->getHitpoints() * 100.0 / (*it)->getMaxHitpoints();
		if ((*it)->isHealAble() && health < lowestHealth) {
			lowestHealth = health;
			weakestUnit = (*it);
		}

		//savefront unit != healer
		xProgess = (*it)->getXAxisProgess();
		if ((*it)->isHealAble() && xProgess > maxXProgess && !(*it)->getAttack()->isHeal()) {
			maxXProgess = xProgess;
			mostProgess = (*it);
		}
	}

	//if all units are full we run to the first unit which is no healer
	if (lowestHealth == 100)
		return mostProgess;
	else
		return weakestUnit;
}

Unit* Unit::getNearestEnemyUnit() {
	Unit* nearestUnit = NULL;
	float lowestDist = FLT_MAX;
	float dist;

	std::list<Unit*> units = Model::getInstance()->getEnemyArmy(_armyType)->getUnits();

	for (std::list<Unit*>::iterator it = units.begin(); it != units.end(); ++it) {
		if ((*it)->getHitpoints() <= 0)
			continue;

		dist = this->distanceTo(*it);
		if (dist < lowestDist) {
			lowestDist = dist;
			nearestUnit = (*it);
		}
	}

	return nearestUnit;
}

bool Unit::attack(Unit* unit) {
	if (this->distanceTo(unit) > this->_attack->getAttackRange())
		return false;

	if (_turnAbleOnAttack)
		setLookingTo(unit->getPositionBW(), false);

	if (_attack->isHeal() && (unit->getArmyType() != this->getArmyType() || unit->getHitpoints() == unit->getMaxHitpoints())) {
		//heal on enemy
		this->doIdle();
		return true;
	}

	if (_currentAnimation != ATTACK)
		_lastAttack = BW_Time::getMilliSecondsCached() - _attackAnimationAttackSync;
	this->setAnimation(ATTACK);
	//if true, there was dmg done
	if (this->_attack->tryAttack(this, unit) && _attackEffectSound != "" && ViewTools::isGameFieldSpriteOnScreen(_sprite))
		SoundHandler::getInstance()->playEffect(_attackEffectSound);

	return true;
}

float Unit::getProgressOfAttackLoopInPercent() {
	float timeBetweenAttacks = (1 / _attack->getAttackSpeed()) * 1000;
	float progressInMilliseconds = BW_Time::getMilliSecondsCached() - _lastAttack;
	float percent = progressInMilliseconds / timeBetweenAttacks;
	if (percent > 1)
		percent = 0;
	return percent;
}

void Unit::applyHandicap(float multiplicator) {
	_hitpoints = _maxHitpoints = _maxHitpoints * multiplicator;
	_attack->applyHandicap(multiplicator);
}

void Unit::turnIntoMaster() {
	_hitpoints = _maxHitpoints = _maxHitpoints * UNITS_TO_MASTER_UNIT_MULT;
	_attack->turnIntoMaster();
	for (std::list<Skill*>::iterator it = _skills.begin(); it != _skills.end(); ++it) {
		(*it)->turnIntoMaster();
	}
	addMasterMark();
}

void Unit::turnIntoUltimate() {
	_hitpoints = _maxHitpoints = _maxHitpoints * UNITS_TO_ULTIMATE_UNIT_MULT;
	_attack->turnIntoUltimate();
	for (std::list<Skill*>::iterator it = _skills.begin(); it != _skills.end(); ++it) {
		(*it)->turnIntoUltimate();
	}
	addChampionMark();
}

void Unit::initAnimationVars() {
	initAnimationVarsAttackSpeed();
	initAnimationVarsRunSpeed();
}

void Unit::initAnimationVarsAttackSpeed() {
	//init attackAnimation vars
	//one cicle per dmg
	_attackAnimationSpeed = (1.0 / _attack->getAttackSpeed()) / _numAttackAnimationPicture;
	//shift the dmg output to the corresponding image
	_attackAnimationAttackSync = _attackAnimationSpeed * 1000 * (_numAttackAnimationPicture - _animationDoDmgPicture);
}
void Unit::initAnimationVarsRunSpeed() {
	_moveAnimationSpeedUsed = _moveAnimationSpeed / _runspeedEffectMultiplier;
}

float Unit::Receivedamage(float damage, DamageType damagetype) {
	float dmg = _armour->realDamage(damage, damagetype);
	setHitpoints(_hitpoints - dmg); //use this couse of lable update
	return dmg;
}

float Unit::recieveHeal(float heal, DamageType healType) {
	heal = _armour->realHeal(heal, healType) * _healRecievingMultiplicator;
	if (_hitpoints > 0)
		setHitpoints(_hitpoints + heal); //use this couse of lable update

	return heal;
}

double Unit::getLastAttack() {
	return _lastAttack;
}

void Unit::setLastAttack(double lastAttack) {
	_lastAttack = lastAttack;
}
