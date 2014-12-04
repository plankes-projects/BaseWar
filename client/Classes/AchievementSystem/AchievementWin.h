/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementWin_H_
#define AchievementWin_H_

#include "Achievement.h"

#include "../Model/GameMode.h"
#include "../Model/Difficulty.h"
#include "AchievementWinsEnum.h"

class AchievementWin: public Achievement {
public:
	AchievementWin(GameMode mode, Difficulty difficulty, AchievementWinsEnum wins);
private:
	bool virtual isAchievedInternCalculation();
	Difficulty _dif;
	GameMode _mode;
	int _wins;
};

#endif /* AchievementWin_H_ */
