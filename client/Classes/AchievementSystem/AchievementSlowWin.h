/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementSlowWin_H_
#define AchievementSlowWin_H_

#include "Achievement.h"
#include "../Model/Difficulty.h"

class AchievementSlowWin: public Achievement {
public:
	AchievementSlowWin(Difficulty dif);
private:
	bool virtual isAchievedInternCalculation();
};

#endif /* AchievementSlowWin_H_ */
