/*
 * Army.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "Army.h"
#include "Model.h"
#include "../Tools/BW_Time.h"
#include "../Tools/Tools.h"

Army::~Army() {
	while (!_projectiles.empty()) {
		delete _projectiles.front();
		_projectiles.pop_front();
	}

	while (!_units.empty()) {
		delete _units.front();
		_units.pop_front();
	}

	while (!_players.empty()) {
		delete _players.front();
		_players.pop_front();
	}
}

Unit* Army::getMostProgressUnit() {
	Unit* mostProgess = NULL;
	float xProgess;
	float maxXProgess = 0;

	for (std::list<Unit*>::iterator it = _units.begin(); it != _units.end(); ++it) {
		xProgess = (*it)->getXAxisProgess();
		if (xProgess > maxXProgess) {
			maxXProgess = xProgess;
			mostProgess = (*it);
		}
	}

	return mostProgess;
}

int Army::getNumberOfUnitsInEnemyField() {
	int numer = 0;

	for (std::list<Unit*>::iterator it = _units.begin(); it != _units.end(); ++it)
		if ((*it)->getXAxisProgess() > 50)
			numer++;

	return numer;
}

Army::Army(ArmyType armyType, BW_Point spawnPoint) {
	this->_armyType = armyType;
	this->_spawnPoint = spawnPoint;
	_towerIsDead = false;
}
void Army::update() {
	for (std::list<Projectile*>::iterator it = _projectiles.begin(); it != _projectiles.end(); ++it)
		(*it)->update();

	for (std::list<Unit*>::iterator it = _units.begin(); it != _units.end(); ++it)
		(*it)->update();

	for (std::list<Player*>::iterator it = _players.begin(); it != _players.end(); ++it)
		(*it)->update();
}
void Army::killUndeadUnits() {
	for (std::list<Unit*>::iterator it = _units.begin(); it != _units.end(); ++it)
		if ((*it)->getHitpoints() <= 0)
			(*it)->kill();
}
std::list<Unit*> Army::getUnits() {
	return _units;
}
void Army::addPlayer(Player* player) {
	player->setArmyTyp(_armyType);
	_players.push_back(player);
}

std::list<Player*> Army::getPlayers() {
	return _players;
}

void Army::spawnUnits() {
	Player* player = _players.front();
	_players.pop_front();
	_players.push_back(player);
	player->spawnUnits();
}

void Army::addUnit(Unit* unit) {
	_units.push_back(unit);

	unit->setArmyType(_armyType);
	int y = unit->getPositionBW().y;
	if (unit->_randomPosStart) {
		// set position of this unit
		int minY = (int) (unit->getContentSize().getHeight() / 2.0);
		int maxY = (int) (Model::getInstance()->getFieldHeigth() - unit->getContentSize().getHeight() / 2.0);

		y = Tools::random(minY, maxY);
	}
	unit->setPosition(BW_Point(_spawnPoint.x + (_armyType == LEFT ? -unit->getContentSize().getWidth() : +unit->getContentSize().getWidth()), y));
	unit->addToDrawLayer();
}
void Army::removeUnit(Unit* unit) {
	_units.remove(unit);
}
BW_Point Army::getSpawnPoint() {
	return _spawnPoint;
}
void Army::addProjectile(Projectile* projectile) {
	projectile->setArmyType(_armyType);
	_projectiles.push_back(projectile);
	projectile->addToDrawLayer();
}
void Army::removeProjectile(Projectile* projectile) {
	_projectiles.remove(projectile);
}
std::list<Projectile*> Army::getProjectiles() {
	return _projectiles;
}
