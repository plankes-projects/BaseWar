/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementFastWin.h"
#include "../Constants.h"
#include "../Tools/Tools.h"

AchievementFastWin::AchievementFastWin(Difficulty dif) :
		Achievement("", "achievementFastWin.png", "In A Hurry", "", 1) {
	_description = "Win under " + Tools::toString(ACHIEVEMENT_FAST_WIN_UNDER_MIN) + " min (" + Tools::toString(dif) + ").";
	_points = 10 * (dif + 1);

	_persistentLoadSaveId = "achievementFastWin" + Tools::toString((int) dif);
	_timeAchieved = getAchievementAchievedPersistent();
}

bool AchievementFastWin::isAchievedInternCalculation() {
	return false;
}
