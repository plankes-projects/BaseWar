/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementFeedback_H_
#define AchievementFeedback_H_

#include "Achievement.h"
#include "../Model/Difficulty.h"

class AchievementFeedback: public Achievement {
public:
	AchievementFeedback();
private:
	bool virtual isAchievedInternCalculation();
};

#endif /* AchievementFastWin_H_ */
