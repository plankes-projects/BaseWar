/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementTutorialFinished_H_
#define AchievementTutorialFinished_H_

#include "Achievement.h"

class AchievementTutorialFinished: public Achievement {
public:
	AchievementTutorialFinished();
private:
	bool virtual isAchievedInternCalculation();
};

#endif /* AchievementTutorialFinished_H_ */
