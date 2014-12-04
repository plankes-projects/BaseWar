/*
 * Base.cpp
 *
 *  Created on: May 18, 2013
 *      Author: planke
 */

#include "cocos2d.h"

#include "Base.h"
#include "../Attacks/ProjectileAttack.h"
#include "../Armours/LifeRegArmour.h"
#include "../Attacks/Projectiles/NormalProjectile.h"
#include "../../SceneControl.h"
#include "../Model.h"
#include "../../Network/NetworkSingleton.h"
#include "../../Network/ServerCommunication.h"
#include "../../Tools/Tools.h"
#include "../../Tools/OfflineLogger.h"
#include "../../Tools/BW_Time.h"
#include "../../Debug/BalanceSingleton.h"
#include "../../AchievementSystem/AchievementFastWin.h"
#include "../../AchievementSystem/AchievementSlowWin.h"
#include "../../AchievementSystem/AchievementPerfectWin.h"
#include "../../AchievementSystem/AchievementSurvival.h"
#include "../../AchievementSystem/AchievementSurvivalEnum.h"
#include "../../AchievementSystem/AchievementMasterGoldSaver.h"
#include "../../Constants.h"
#include "../Effects/UpgradeUnitEffect.h"
#include "../../Sound/SoundHandler.h"

Base::~Base() {
}

Unit* Base::clone() {
	return new Base();
}

Base::Base() :
		Unit("baseUnit.png", 600, 3600, new ProjectileAttack(new ProjectileFactory(new NormalProjectile(true)), 200, 3, 1000),
				new LifeRegArmour(40), std::list<Skill*>(), 0, 0, false) {
	initAnimations(); //loading the pictures and set the animations
	_hitBox = BW_Rect(300, 300);
	setOpacity(200);
	_turnAbleOnAttack = false;
	_isHealAble = false;

	//upgrade for 10 min
	//every 1 min
	_effects.push_back(new UpgradeUnitEffect(10 * 60 * 1000, 1.5 * 60 * 1000));
	_attackEffectSound = "sound_attack_tower_base";
}

void Base::onDie() {
	bool won;
	if (Model::getInstance()->getStartedAs() == PVP || Model::getInstance()->getThisPlayer()->getArmyTyp() == _armyType)
		won = false;
	else
		won = true;

	std::string s = won ? "sound_game_won" : "sound_game_lost";
	SoundHandler::getInstance()->playEffect(s);
	SoundHandler::getInstance()->playBackground("sound_menu1");

	int sec = BW_Time::getMilliSecondsOverCached() / 1000 + 1;
	int min = sec / 60;

	std::string winInfo = (Model::getInstance()->getThisPlayer()->getArmyTyp() != _armyType ? "WIN" : "LOSE");
	if (Model::getInstance()->getStartedAs() == NETWORK) {
		ServerCommunication().sendGameInfo(winInfo);
	}

	if(Model::getInstance()->getStartedAs() == PVP || Model::getInstance()->getStartedAs() == PVA){
		OfflineLogger::getInstance()->logInformationForCurrentGame(winInfo);
	}

	GameStatistics gs = GameStatistics();
	Difficulty diff = Model::getInstance()->getDifficulty();
	switch (Model::getInstance()->getStartedAs()) {
	case PVP:
		gs.increaseLocalMultiplayerGamesFinished();
		break;
	case PVA:
		if (diff == DIF_HELL) {
			gs.updateBestSurviveInSec(BW_Time::getMilliSecondsOverCached() / 1000 + 1);

			if (won || min >= 41)
				AchievementSurvival(SURVIVLE_41).setAchievedNow();
			if (won || min >= 23)
				AchievementSurvival(SURVIVLE_23).setAchievedNow();
			if (won || min >= 11)
				AchievementSurvival(SURVIVLE_11).setAchievedNow();
			if (won || min >= 5)
				AchievementSurvival(SURVIVLE_5).setAchievedNow();
		}

		if (won) {
			gs.increaseLocalGamesWon();

			if (Model::getInstance()->getThisPlayer() != NULL && Model::getInstance()->getThisPlayer()->getGoldPeak() >= ACHIEVEMENT_MASTER_SAVER_GOLD)
				AchievementMasterGoldSaver(diff).setAchievedNow();

			if (min < ACHIEVEMENT_FAST_WIN_UNDER_MIN)
				AchievementFastWin(diff).setAchievedNow();
			else if (min >= ACHIEVEMENT_SLOW_WIN_AFTER_MIN)
				AchievementSlowWin(diff).setAchievedNow();

			if (!Model::getInstance()->getEnemyArmy(_armyType)->isTowerIsDead())
				AchievementPerfectWin(diff).setAchievedNow();

		} else {
			gs.increaseLocalGamesLost();
		}
		break;
	case NETWORK:
		if (won) {
			gs.increaseNetworkGamesWon();
		} else {
			gs.increaseNetworkGamesLost();
		}
		break;
	default:
		;
		//this is not possible to occur
	}

#ifdef DEBUG
	if (Model::getInstance()->getGameMode() == BALANCE_TEST) {
		//print balance statistic
		BalanceSingleton* b = BalanceSingleton::getInstance();
		b->increaseRound();
		if (_armyType == RIGHT)
		b->increaseRace1Win();
		else
		b->increaseRace2Win();

		int sec = BW_Time::getMilliSecondsOverCached() / 1000 + 1;
		b->addDuration(sec);
		CCLOG("%s", b->getInformation().c_str());

		SceneControl::replaceScene(SceneControl::startMenuScene(), false);
		return;
	}
#endif
	gs.increaseSecondsPlayedBy(BW_Time::getMilliSecondsOverCached() / 1000 + 1);
	SceneControl::replaceScene(SceneControl::gameOverScene(won), false);
}

void Base::fillMoveAnimationPictures() {
}
void Base::fillIdleAnimationPictures() {
}

void Base::fillAttackAnimationPictures() {
}
