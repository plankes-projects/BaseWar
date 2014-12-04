/*
 * Tower.cpp
 *
 *  Created on: May 19, 2013
 *      Author: planke
 */

#include "Tower.h"
#include "../Attacks/ProjectileAttack.h"
#include "../Attacks/Projectiles/NormalProjectile.h"
#include "../Armours/LifeRegArmour.h"
#include "../Player.h"
#include "../Model.h"
#include "../../Constants.h"
#include "../Effects/UpgradeUnitEffect.h"
#include "../../Tools/Tools.h"
#include "../../Tools/OfflineLogger.h"
#include "../../Network/ServerCommunication.h"
#include "../../Tools/BW_Time.h"

#include "cocos2d.h"

Tower::~Tower() {
}

Unit* Tower::clone() {
	return new Tower();
}

Tower::Tower() :
		Unit("tower.png", 400, 1800, new ProjectileAttack(new ProjectileFactory(new NormalProjectile(true)), 80, 3, 1000), new LifeRegArmour(20),
				std::list<Skill*>(), 0, 0, false) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(30, 90);
	setOpacity(200);
	_turnAbleOnAttack = false;
	_isHealAble = false;

	//upgrade for 10 min
	//every 1 min
	_effects.push_back(new UpgradeUnitEffect(10 * 60 * 1000, 1 * 60 * 1000));
	_attackEffectSound = "sound_attack_tower_base";
}

void Tower::onDie() {
	//all enemy players gets gold
	std::list<Player*> players = Model::getInstance()->getEnemyArmy(_armyType)->getPlayers();

	for (std::list<Player*>::iterator it = players.begin(); it != players.end(); ++it)
		(*it)->increaseGoldBy(GOLD_ON_TOWER_KILL);

	Model::getInstance()->getMyArmy(_armyType)->setTowerIsDead(true);

	bool myTower = Model::getInstance()->getThisPlayer()->getArmyTyp() == _armyType;
	std::string winInfo = (myTower ? "MY TOWER DOWN" : "OTHER TOWER DOWN");
	if (Model::getInstance()->getGameMode() == NETWORK) {
		ServerCommunication().sendGameInfo(winInfo);
	}

	if(Model::getInstance()->getStartedAs() == PVP || Model::getInstance()->getStartedAs() == PVA){
		OfflineLogger::getInstance()->logInformationForCurrentGame(winInfo);
	}
}

void Tower::fillMoveAnimationPictures() {
}
void Tower::fillIdleAnimationPictures() {
}
void Tower::fillAttackAnimationPictures() {
}
