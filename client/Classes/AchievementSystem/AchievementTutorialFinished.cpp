/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementTutorialFinished.h"

AchievementTutorialFinished::AchievementTutorialFinished() :
		Achievement("achievementTutorialFinished", "achievementTutorialFinished.png", "Smart Gamer", "Complete the tutorial.", 10) {
}

bool AchievementTutorialFinished::isAchievedInternCalculation() {
	return false;
}
