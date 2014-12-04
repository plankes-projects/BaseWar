/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementPerfectWin_H_
#define AchievementPerfectWin_H_

#include "Achievement.h"
#include "../Model/Difficulty.h"

class AchievementPerfectWin: public Achievement {
public:
	AchievementPerfectWin(Difficulty dif);
private:
	bool virtual isAchievedInternCalculation();
};

#endif /* AchievementStartedBaseWar_H_ */
