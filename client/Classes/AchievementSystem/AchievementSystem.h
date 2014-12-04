/*
 * Tools.h
 *
 *  Created on: 11.01.2014
 *      Author: Planke
 */

#ifndef AchievementSystem_H_
#define AchievementSystem_H_

#include <vector>
#include "Achievement.h"

#include "cocos2d.h"
using namespace cocos2d;

class AchievementSystem {
public:
	AchievementSystem();
	virtual ~AchievementSystem();
	std::vector<Achievement*> getAchievements();

	int getMaxPoints(){
		return _maxPoints;
	}

	int getPoints(){
		return _points;
	}

private:
	std::vector<Achievement*> _achievements;
	void sortAchievements();

	int _points;
	int _maxPoints;
};

#endif /* AchievementSystem_H_ */
