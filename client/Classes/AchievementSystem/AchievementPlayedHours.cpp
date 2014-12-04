/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementPlayedHours.h"
#include "../Tools/Tools.h"
#include "../Tools/GameStatistics.h"

AchievementPlayedHours::AchievementPlayedHours(AchievementPlayedHoursEnum hours) :
		Achievement("", "", "Loyal Gamer", "", 20) {

	switch (hours) {
	case PLAYED_3:
		_hours = 3;
		break;
	case PLAYED_11:
		_hours = 11;
		break;
	case PLAYED_17:
		_hours = 17;
		break;
	case PLAYED_23:
		_hours = 23;
		break;
	case PLAYED_103:
		_hours = 103;
		break;
	}

	_description = "Play " + Tools::toString(_hours) + " hours.";
	_image = "achievementPlayedHours" + Tools::toString(_hours) + ".png";

	_persistentLoadSaveId = "achievementPlayedHours" + Tools::toString((int) hours);
	_timeAchieved = getAchievementAchievedPersistent();
}

bool AchievementPlayedHours::isAchievedInternCalculation() {
	return ((int) (GameStatistics().getSecondsPlayed() / 3600)) >= _hours;
}
