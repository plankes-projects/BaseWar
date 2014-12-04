/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementFastWin_H_
#define AchievementFastWin_H_

#include "Achievement.h"
#include "../Model/Difficulty.h"

class AchievementFastWin: public Achievement {
public:
	AchievementFastWin(Difficulty dif);
private:
	bool virtual isAchievedInternCalculation();
};

#endif /* AchievementFastWin_H_ */
