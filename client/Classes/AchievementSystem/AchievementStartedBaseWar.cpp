/*
 * Tools.cpp
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#include "AchievementStartedBaseWar.h"

AchievementStartedBaseWar::AchievementStartedBaseWar() :
		Achievement("achievementStartedBaseWar", "achievementStartedBaseWar.png", "First time", "Welcome to BaseWar!", 1) {
}

bool AchievementStartedBaseWar::isAchievedInternCalculation() {
	return true;
}
