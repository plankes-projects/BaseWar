/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementStartedBaseWar_H_
#define AchievementStartedBaseWar_H_

#include "Achievement.h"

class AchievementStartedBaseWar: public Achievement {
public:
	AchievementStartedBaseWar();
private:
	bool virtual isAchievedInternCalculation();
};

#endif /* AchievementStartedBaseWar_H_ */
