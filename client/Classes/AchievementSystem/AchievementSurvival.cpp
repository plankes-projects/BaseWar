/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementSurvival.h"
#include "../Tools/Tools.h"

AchievementSurvival::AchievementSurvival(AchievementSurvivalEnum min) :
		Achievement("", "", "Survivor", "", 20) {

	switch (min) {
	case SURVIVLE_5:
		_min = 5;
		break;
	case SURVIVLE_11:
		_min = 11;
		break;
	case SURVIVLE_23:
		_min = 23;
		break;
	case SURVIVLE_41:
		_min = 41;
		break;
	}

	_description = "Survive " + Tools::toString(_min) + " minutes at survival difficulty.";
	_image = "achievementSurvival" + Tools::toString(_min) + ".png";

	_persistentLoadSaveId = "cachievementSurvival" + Tools::toString((int) min);
	_timeAchieved = getAchievementAchievedPersistent();
}

bool AchievementSurvival::isAchievedInternCalculation() {
	return false;
}
