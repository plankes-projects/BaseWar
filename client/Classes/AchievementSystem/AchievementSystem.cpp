/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementSystem.h"

#include "../Model/GameMode.h"
#include "../Model/Difficulty.h"
#include "AchievementPlayedHoursEnum.h"
#include "AchievementSurvivalEnum.h"

#include "AchievementWin.h"
#include "AchievementStartedBaseWar.h"
#include "AchievementPerfectWin.h"
#include "AchievementFastWin.h"
#include "AchievementSlowWin.h"
#include "AchievementPlayedHours.h"
#include "AchievementSurvival.h"
#include "AchievementMasterGoldSaver.h"
#include "AchievementTutorialFinished.h"
#include "AchievementFeedback.h"

AchievementSystem::AchievementSystem() {
	//generate all achievements
	_achievements.push_back(new AchievementStartedBaseWar());
	_achievements.push_back(new AchievementTutorialFinished());
	_achievements.push_back(new AchievementFeedback());

	for (int hours = PLAYED_3; hours <= PLAYED_103; hours++) {
		_achievements.push_back(new AchievementPlayedHours((AchievementPlayedHoursEnum) hours));
	}

	for (int min = SURVIVLE_5; min <= SURVIVLE_41; min++) {
		_achievements.push_back(new AchievementSurvival((AchievementSurvivalEnum) min));
	}

	for (int wins = WIN_1; wins <= WIN_53; wins++) {
		_achievements.push_back(new AchievementWin(PVP, DIF_EASY, (AchievementWinsEnum) wins)); //diff not used
		_achievements.push_back(new AchievementWin(NETWORK, DIF_EASY, (AchievementWinsEnum) wins)); //diff not used

		for (int dif = DIF_EASY; dif <= DIF_VERYHARD; dif++) {
			_achievements.push_back(new AchievementWin(PVA, (Difficulty) dif, (AchievementWinsEnum) wins));
		}
	}

	for (int dif = DIF_EASY; dif <= DIF_VERYHARD; dif++) {
		_achievements.push_back(new AchievementPerfectWin((Difficulty) dif));
		_achievements.push_back(new AchievementFastWin((Difficulty) dif));
		_achievements.push_back(new AchievementSlowWin((Difficulty) dif));
		_achievements.push_back(new AchievementMasterGoldSaver((Difficulty) dif));
	}

	// added all achievements ----------------------------------

	_maxPoints = _points = 0;
	for (std::vector<Achievement*>::iterator achi = _achievements.begin(); achi != _achievements.end(); ++achi) {
		Achievement* ach = (*achi);
		ach->updateAchievement();
		int p = ach->getPoints();
		_maxPoints += p;
		if (ach->isAchieved())
			_points += p;
	}

	sortAchievements();
}

void AchievementSystem::sortAchievements() {

//stretegic: achievedTime > points

	std::vector<Achievement*> sortedAchieved;
	std::vector<Achievement*> sortedNotAchieved;
	std::vector<Achievement*>::iterator insert;

	for (std::vector<Achievement*>::iterator achi = _achievements.begin(); achi != _achievements.end(); ++achi) {
		Achievement* ach = *achi;
		if (ach->isAchieved()) {
			for (insert = sortedAchieved.begin(); insert != sortedAchieved.end(); ++insert) {
				if ((*insert)->getTimeAchieved() < ach->getTimeAchieved())
					break;
			}
			sortedAchieved.insert(insert, ach);
		} else {
			for (insert = sortedNotAchieved.begin(); insert != sortedNotAchieved.end(); ++insert) {
				if ((*insert)->getPoints() > ach->getPoints())
					break;
			}
			sortedNotAchieved.insert(insert, ach);
		}
	}

//merge the 2 vectors
	_achievements = sortedAchieved;
	for (insert = sortedNotAchieved.begin(); insert != sortedNotAchieved.end(); ++insert) {
		_achievements.push_back(*insert);
	}

}

AchievementSystem::~AchievementSystem() {
	for (std::vector<Achievement*>::iterator a = _achievements.begin(); a != _achievements.end(); ++a) {
		delete (*a);
	}

//no need to delete _newAchievements
// because every achievement is already in _achievements
}

std::vector<Achievement*> AchievementSystem::getAchievements() {
	return _achievements;
}
