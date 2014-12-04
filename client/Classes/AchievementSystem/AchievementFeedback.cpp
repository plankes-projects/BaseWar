/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementFeedback.h"
#include "../Constants.h"
#include "../Tools/Tools.h"

AchievementFeedback::AchievementFeedback() :
		Achievement("", "achievementFeedback.png", "Very useful", "", 1) {
	_description = "Give some feedback.";
	_points = 20;

	_persistentLoadSaveId = "AchievementFeedback";
	_timeAchieved = getAchievementAchievedPersistent();
}

bool AchievementFeedback::isAchievedInternCalculation() {
	return false;
}
