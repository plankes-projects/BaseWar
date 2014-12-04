/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementMasterGoldSaver_H_
#define AchievementMasterGoldSaver_H_

#include "Achievement.h"
#include "../Model/Difficulty.h"

class AchievementMasterGoldSaver: public Achievement {
public:
	AchievementMasterGoldSaver(Difficulty dif);
private:
	bool virtual isAchievedInternCalculation();
};

#endif /* AchievementMasterGoldSaver_H_ */
