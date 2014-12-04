/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementPerfectWin.h"
#include "../Tools/Tools.h"

AchievementPerfectWin::AchievementPerfectWin(Difficulty dif) :
		Achievement("", "achievementPerfectWin.png", "Perfect Win!", "", 1) {
	_description = "Do not lose your tower (" + Tools::toString(dif) + ").";
	_points = 10 * (dif + 1);

	_persistentLoadSaveId = "achievementAPerfectWin" + Tools::toString((int) dif);
	_timeAchieved = getAchievementAchievedPersistent();
}

bool AchievementPerfectWin::isAchievedInternCalculation() {
	return false;
}
