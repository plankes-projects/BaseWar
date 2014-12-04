/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementMasterGoldSaver.h"
#include "../Constants.h"
#include "../Tools/Tools.h"

AchievementMasterGoldSaver::AchievementMasterGoldSaver(Difficulty dif) :
		Achievement("", "achievementMasterGoldSaver.png", "Bank", "", 1) {
	_description = "Get more than " + Tools::toString(ACHIEVEMENT_MASTER_SAVER_GOLD) + " gold and win (" + Tools::toString(dif) + ").";
	_points = 10 * (dif + 1);

	_persistentLoadSaveId = "achievementMasterGoldSaver" + Tools::toString((int) dif);
	_timeAchieved = getAchievementAchievedPersistent();
}

bool AchievementMasterGoldSaver::isAchievedInternCalculation() {
	return false;
}
