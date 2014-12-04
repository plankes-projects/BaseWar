/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementSurvival_H_
#define AchievementSurvival_H_

#include "Achievement.h"
#include "AchievementSurvivalEnum.h"

class AchievementSurvival: public Achievement {
public:
	AchievementSurvival(AchievementSurvivalEnum min);
private:
	bool virtual isAchievedInternCalculation();
	int _min;
};

#endif /* AchievementSurvival_H_ */
