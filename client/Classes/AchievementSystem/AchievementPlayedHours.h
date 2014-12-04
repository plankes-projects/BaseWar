/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementPlayedHours_H_
#define AchievementPlayedHours_H_

#include "Achievement.h"
#include "AchievementPlayedHoursEnum.h"

class AchievementPlayedHours: public Achievement {
public:
	AchievementPlayedHours(AchievementPlayedHoursEnum hours);
private:
	bool virtual isAchievedInternCalculation();
	int _hours;
};

#endif /* AchievementPlayedHours_H_ */
