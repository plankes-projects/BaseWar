/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementSlowWin.h"
#include "../Constants.h"
#include "../Tools/Tools.h"

AchievementSlowWin::AchievementSlowWin(Difficulty dif) :
		Achievement("", "achievementSlowWin.png", "Slow Down", "", 1) {
	_description = "Win after " + Tools::toString(ACHIEVEMENT_SLOW_WIN_AFTER_MIN) + " min (" + Tools::toString(dif) + ").";
	_points = 10 * (dif + 1);

	_persistentLoadSaveId = "achievementSlowWin" + Tools::toString((int) dif);
	_timeAchieved = getAchievementAchievedPersistent();
}

bool AchievementSlowWin::isAchievedInternCalculation() {
	return false;
}
