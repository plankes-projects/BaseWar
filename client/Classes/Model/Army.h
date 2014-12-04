/*
 * Army.h
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#ifndef ARMY_H_
#define ARMY_H_

#include <list>
#include "ArmyType.h"
#include "../Tools/BW_Point.h"
#include "Units/Unit.h"
#include "Attacks/Projectiles/Projectile.h"
#include "../Model/Player.h"

class Army {
public:
	Army();
	virtual ~Army();
	Army(ArmyType armyType, BW_Point spawnPoint);
	void update();
	void killUndeadUnits();
	std::list<Unit*> getUnits();
	void addUnit(Unit* unit);
	void removeUnit(Unit* unit);
	BW_Point getSpawnPoint();
	void addProjectile(Projectile* projectile);
	void removeProjectile(Projectile* projectile);
	std::list<Projectile*> getProjectiles();
	void addPlayer(Player* player);
	void spawnUnits();
	std::list<Player*> getPlayers();
	int getNumberOfUnitsInEnemyField();

	Unit* getMostProgressUnit();

	bool isTowerIsDead() const {
		return _towerIsDead;
	}

	void setTowerIsDead(bool towerIsDead) {
		_towerIsDead = towerIsDead;
	}

private:

	BW_Point _spawnPoint;
	ArmyType _armyType;
	std::list<Unit*> _units;
	std::list<Projectile*> _projectiles;
	std::list<Player*> _players;
	bool _towerIsDead;
};

#endif /* ARMY_H_ */
